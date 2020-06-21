/*
 *  Author: Nicholas Heim
 * Contact: nickheim28@gmail.com
 * Created: June 20, 2020
 * Purpose: A C++ implementation of the 0-1 Knapsack algorithm using vectors.
 *          Note: It cannot handle multiple items of the same weight and
 *          different values currently.
 *          https://en.wikipedia.org/wiki/Knapsack_problem
 */

// Preprocessor Directives
#include <map>
#include <algorithm>
#include <iterator>
#include <cstdio>
#include <vector>
#include <iostream>
#include <iomanip>

// Global Constants
// SIZE denotes the number of items to choose from (for now)
size_t SIZE = 5;
// Need the max weight of the knapsack
int MAX_WEIGHT = 10;

int main(int argc, char const *argv[])
{
   // Need weights and values of the items and the number of items
   // Note: It will be assumed that there are no items of weight zero.
   // TODO: Make dynamic and delete after the map is allocated and weights
   //       are entered into the first column
   std::vector<int> weights, values;
   weights.reserve(SIZE); values.reserve(SIZE);

   // These are placeholder vectors for testing.
   for(size_t i = 0; i < SIZE; ++i)
   {
      weights.push_back(i + 1);        // {1, 2, 3, 4, 5}
      values.push_back((i + 1) * 2);   // {2, 4, 6, 8, 10}
   }

   // Set up the map of values from the vector
   // (Key, Value) = (weight, value)
   std::map<int, int> pairs;
   for(size_t i = 0; i < SIZE; ++i)
      pairs[weights[i]] = values[i];
   
   // Set up the matrix of values: 
   // Columns will be the different weights,
   // Rows will be the weights (and values) of the items.
   int **knapsack = new int*[SIZE];
   for(size_t i = 0; i < SIZE; ++i)
      knapsack[i] = new int[MAX_WEIGHT + 2];

   // For reference, put the weights in the first column.
   for(size_t i = 0; i < SIZE; ++i) 
   {
      knapsack[i][0] = weights[i];
      knapsack[i][1] = 0;
   }

   // First row, starting at the third column (others are done)
   // The value will be zero until we reach a the minimum weight
   // TODO: Make the array start with the minimum weight and
   //       a column of zeros.
   for(size_t i = 2; i < knapsack[0][0]; ++i)
      knapsack[0][i] = 0;
   
   for(size_t i = knapsack[0][0] + 1; i < MAX_WEIGHT + 2; ++i)
      knapsack[0][i] = values[0];
   
   // Generalization of second row:
   for(size_t i = 1; i < SIZE; ++i)
   {
      for(size_t j = 2; j < knapsack[i][0] + 2; ++j)
         knapsack[i][j] = knapsack[i - 1][j];

      for(size_t j = knapsack[i][0] + 1; j < MAX_WEIGHT + 2; ++j)
         knapsack[i][j] = std::max(pairs[knapsack[i][0]] + knapsack[i - 1][j - knapsack[i][0]],
                                   knapsack[i - 1][j]);
   }

   for(size_t i = 0; i < SIZE; ++i)
   {
      for(size_t j = 0; j < MAX_WEIGHT + 2; ++j)
      {
         std::cout << std::setw(2) << std::setfill('0') << knapsack[i][j] << " ";
      }
      std::cout << std::endl;
   }
   
   std::cout << "Max value: " << (knapsack[SIZE - 1][MAX_WEIGHT + 1]) << std::endl;

   // Memory Cleanup
   for(size_t i = 0; i < SIZE; ++i)
      delete [] knapsack[i];
   delete [] knapsack;
   
   return 0;
}