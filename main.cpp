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
#include <algorithm>
#include <iterator>
#include <utility>
#include <cstdio>
#include <vector>

// Global Constants
// SIZE denotes the number of items to choose from (for now)
size_t SIZE = 10;
// Need the max weight of the knapsack
int MAX_WEIGHT = 165;

// Prototypes
std::vector<std::pair<int, int>>* genVector(std::vector<int>,
                                           std::vector<int>);
void knapsack(std::vector<std::pair<int, int>> *);
void const save(int **);


int main(int argc, char const *argv[])
{
   // Need weights and values of the items and the number of items
   // Note: It will be assumed that there are no items of weight zero.
   std::vector<int> weights, values;
   weights.reserve(SIZE); values.reserve(SIZE);

   // Hard coded values for testing and demonstration. Best solution is 309. 
   weights = {23, 29, 31, 38, 44, 53, 63, 82, 85, 89};
   values =  {92, 49, 57, 43, 68, 60, 67, 72, 84, 87};

   // TODO: Add a loading function for file processing.
   knapsack(genVector(weights, values));
   // TODO: Bottom up calculation to find out which items end up in the sack.

   return 0;
}

/*
 *     Pre: Requires that the weights and values be loaded into vectors of ints.
 *          Also requires that the SIZE variable is set correctly (top of file).
 *    Post: Returns a sorted (by weight) vector of pairs for use in the algo. 
 * Purpose: This will sort the (weight, value) pairs in ascending order by
 *          weight. This will be used in the knapsack function and is important
 *          for the calculation for the first line of the knapsack array. 
 */
std::vector<std::pair<int, int>>* genVector(std::vector<int> weights, std::vector<int> values)
{
   std::vector<std::pair<int, int>> *pairs = new std::vector<std::pair<int, int>>;
   for(size_t i = 0; i < SIZE; ++i)
      pairs->push_back(std::make_pair(weights[i], values[i]));
   std::sort(pairs->begin(), pairs->end());
   return pairs;
}

/*
 *     Pre: Requires that the genVector function has been run and the result
 *          is passed to this function. Also requires that the global constants
 *          SIZE and MAX_WEIGHT are correctly set. 
 *    Post: The array data will be saved to a .csv file per the save function.
 * Purpose: This function calculates the best value that can be held in the
 *          sack at one time using whole items. 
 */
void knapsack(std::vector<std::pair<int, int>> *pairs)
{
   // Set up the matrix of values:
   // Columns will be the different weights,
   // Rows will be the weights (and values) of the items.
   int **sack = new int*[SIZE];
   for(size_t i = 0; i < SIZE; ++i)
      sack[i] = new int[MAX_WEIGHT + 2];

   // For reference, put the weights in the first column.
   for(size_t i = 0; i < SIZE; ++i) 
   {
      sack[i][0] = (*pairs)[i].first;
      sack[i][1] = 0;
   }

   // First row, starting at the third column (others are done)
   // The value will be zero until we reach the weight of the first item choice
   for(size_t i = 2; i < sack[0][0]; ++i)
      sack[0][i] = 0;
   
   for(size_t i = sack[0][0] + 1; i < MAX_WEIGHT + 2; ++i)
      sack[0][i] = (*pairs)[0].second;
   
   // Second row and onward.
   for(size_t i = 1; i < SIZE; ++i)
   {
      for(size_t j = 2; j < sack[i][0] + 2; ++j)
         sack[i][j] = sack[i - 1][j];

      for(size_t j = sack[i][0] + 1; j < MAX_WEIGHT + 2; ++j)
         sack[i][j] = std::max((*pairs)[i].second + sack[i - 1][j - sack[i][0]],
                                sack[i - 1][j]);
   }

   save(sack);

   // Memory Cleanup
   for(size_t i = 0; i < SIZE; ++i)
      delete [] sack[i];
   delete [] sack;
   delete pairs;
}

/*
 *     Pre: Requires that a pointer to a 2D dynamic array be passed and that
 *          the values of SIZE and MAX_WEIGHT are set. 
 *    Post: The array has not been changed and has been written to a file by the 
 *          name of "data.csv". 
 * Purpose: This function is meant to save the final array generated by the
 *          algorithm to the specified file. The best value will be in the
 *          bottom right of the saved array. 
 */
void const save(int **sack)
{
   // Output of the data to a file.
   FILE *save = fopen("data.csv", "w");
   for(size_t i = 0; i < SIZE; ++i)
   {
      for(size_t j = 0; j < MAX_WEIGHT + 1; ++j)
      {
         fprintf(save, "%d,", sack[i][j]);
      }
      fprintf(save, "%d\n", sack[i][MAX_WEIGHT + 1]);
   }   
   fclose(save);
}