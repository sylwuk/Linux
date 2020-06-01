/*
 * mymalloc.c
 *
 *  Created on: May 29, 2020
 *      Author: sylwek
 */
#include <sys/types.h>     /* Type definitions used by many programs */
#include <stdio.h>         /* Standard I/O functions */
#include <stdlib.h>        /* Prototypes of commonly used library functions,
                            plus EXIT_SUCCESS and EXIT_FAILURE constants */
#include <unistd.h>        /* Prototypes for many system calls */
#include "tlpi_hdr.h"

#define FREE_THRS 0x20000  /* 128KB threshold when free will call sbrk to lower program break*/
#define MAX_ALLOCS 1000000

struct memnode {
   size_t size;
   struct memnode *prev;
   struct memnode *next;
};

/* Root node of the freed nodes memory list. */
static struct memnode *memlist = NULL;

void* mymalloc(size_t size) {
   struct memnode *nbrk, *node = memlist;;

   // Search for a node in memory list with enough size
   while(node != NULL && node->size < size) {
      node = node->next;
   }
   if(node == NULL) {
      /* Haven't found a node on the list with sufficient size.
       * Move program break by size bytes and
       * add additional bytes for storing size of the block */
      nbrk = (struct memnode *)sbrk((intptr_t)size + sizeof(size_t));
      if(nbrk == (void *)-1) {
         /* Sbrk was unable to move program break. Return.
          * Errno is set to ENOMEM. */
         return NULL;
      }
      nbrk->size = size + sizeof(size_t);
   } else {                /* Found a node with sufficient size */
      nbrk = node;
      if(node->size > size && (node->size - size) > sizeof(struct memnode)) {
         /* Split the node if it is bigger than the requested block
          * and update the list with the new smaller leftover node. */
         node += size;
         node->size = node->size - size;  /* node->size already takes sizeof(size_t) into account */
         node->next = nbrk->next;
         node->prev = nbrk->prev;
         if(nbrk->prev != NULL) {
            nbrk->prev->next = node;
         }
      } else {
         /* The node is exactly the same size as the requested block.
          * Remove the node from the list. */
         if(node->prev != NULL) {
            node->prev->next = node->next;
         }
         if(node->next != NULL) {
            node->next->prev = node->prev;
         }
      }
   }
   return (void *)((char *)nbrk + sizeof(size_t));
}

void myfree(void *ptr) {
   struct memnode *nbrk = (struct memnode *)((char *)ptr - sizeof(size_t));

   if(memlist == NULL) {
      /* If the list is empty add the node to the list and make it a head
       * or lower the program break if the size of the block is over the threshold. */
      if(nbrk->size < FREE_THRS) {
         memlist = nbrk;
         memlist->prev = NULL;
         memlist->next = NULL;
      } else {
         sbrk(-(nbrk->size));
      }
   } else {
      /* List has at least one element. */
      struct memnode *prev = NULL, *node = memlist;

      /* Search for the first node past currently freed block */
      while(node != NULL && node < nbrk) {
         prev = node;
         node = node->next;
      }

      /* Freed block is before in memory than the last found node. */
      nbrk->next = node;
      nbrk->prev = prev;
      if(prev == NULL) {
         /* Update the list head if the freed block is before the current list head in memory */
         memlist = nbrk;
      } else {
         prev->next = nbrk;
      }
      if(node != NULL) {
         node->prev = nbrk;
      }
      /* Check if the program break can be moved. */
      node = nbrk;

      /* If currently freed node is the last on the list
       * set the sum to its size since the forward checking while loop
       * will not execute.*/
      size_t sum = nbrk->next == NULL ? nbrk->size : 0;
      while(node != NULL && ((char *)node + node->size) == (char *)node->next) {
         /* Sum the memory of nodes after the current node
          * that are consecutive in memory. */
         sum += node->size;
         node = node->next;
      }
      prev = nbrk;
      node = nbrk->prev;
      while(node != NULL && ((char *)node + node->size) == (char *)node->next) {
         /* Sum previous consecutive memory nodes. */
         sum += node->size;
         prev = node;
         node = node->prev;
      }

      node = prev;
      if((void *)((char *)node + sum) >= sbrk(0) && sum > FREE_THRS) {
         /* Sum of the sizes of last nodes is higher than free threshold.
          * Move program break back. */
         if(node->prev == NULL) {
            /* Reached the beginning of the list.
             * Destroy the list. */
            memlist = NULL;
         }
         sbrk(-sum);
      }
   }
   return;
}

int main(int argc, char *argv[]) {
   char *ptr[MAX_ALLOCS];
   int freeStep, freeMin, freeMax, blockSize, numAllocs, j;

   printf("\n");

   if(argc < 3 || strcmp(argv[1], "--help") == 0) {
      usageErr("%s num-allocs block-size [step [min [max]]]\n", argv[0]);
   }

   numAllocs = getInt(argv[1], GN_GT_O, "num-allocs");
   if(numAllocs > MAX_ALLOCS) cmdLineErr("num-allocs > %d\n", MAX_ALLOCS);

   blockSize = getInt(argv[2], GN_GT_O | GN_ANY_BASE, "block-size");

   freeStep = (argc > 3) ? getInt(argv[3], GN_GT_O, "step") : 1;
   freeMin = (argc > 4) ? getInt(argv[4], GN_GT_O, "min") : 1;
   freeMax = (argc > 5) ? getInt(argv[5], GN_GT_O, "max") : numAllocs;

   if(freeMax > numAllocs) cmdLineErr("free-max > num-Allocs\n");

   printf("Initial program break:             %10p\n", sbrk(0));
   printf("Allocating %d*%d bytes\n", numAllocs, blockSize);

   for(j = 0; j < numAllocs; ++j) {
      ptr[j] = mymalloc(blockSize);
      if(ptr[j] == NULL) errExit("malloc");
   }

   printf("Program break is now:              %10p\n", sbrk(0));
   printf("Freeing blocks from %d to %d in steps of %d\n", freeMin, freeMax, freeStep);

   for(j = freeMin - 1; j < freeMax; j += freeStep) {
      myfree(ptr[j]);
   }

   printf("After free() program break is:     %10p\n", sbrk(0));

   exit(EXIT_SUCCESS);
}
