/*
 *  Author: Nicholas Heim
 * Contact: nickheim28@gmail.com
 * Created: June 20, 2020
 * Purpose: A C++ implementation of the 0-1 Knapsack algorithm using vectors.
 *          https://en.wikipedia.org/wiki/Knapsack_problem
 */

// Preprocessor Directives
#include <algorithm>
#include <utility>
#include <vector>

// Prototypes
std::vector<std::pair<int, int>>* genPairs(std::vector<int> &,
   std::vector<int> &, const int);
int ** knapsack(std::vector<std::pair<int, int>> *, const int, const int);

int main(int argc, char const *argv[])
{
   // Need weights and values of the items and the number of items
   const int size = 10, max = 165;
   std::vector<int> weights, values;
   weights.reserve(size); values.reserve(size);

   // Hard coded values for testing and demonstration. Best solution is 309. 
   weights = {23, 29, 31, 38, 44, 53, 63, 82, 85, 89};
   values =  {92, 49, 57, 43, 68, 60, 67, 72, 84, 87};

   auto pairs = genPairs(weights, values, size);
   auto sack = knapsack(pairs, size, max);
   // TODO: Bottom up calculation to find out which items end up in the sack.

   return 0;
}

/*
 *     Pre: Requires that the weights and values be loaded into vectors of ints.
 *          Also requires that a size variable is passed.
 *    Post: Returns a sorted (by weight) vector of pairs for use in the algo. 
 * Purpose: This will sort the (weight, value) pairs in ascending order by
 *          weight. This will be used in the knapsack function and is important
 *          for the calculation for the first line of the knapsack array. 
 */
std::vector<std::pair<int, int>>* genPairs(std::vector<int> &weights, std::vector<int> &values, const int size)
{
   std::vector<std::pair<int, int>> *pairs = new std::vector<std::pair<int, int>>;
   for(size_t i = 0; i < size; ++i)
      pairs->push_back(std::make_pair(weights[i], values[i]));
   std::sort(pairs->begin(), pairs->end());
   return pairs;
}

/*
 *     Pre: Requires that the genVector function has been run and the result
 *          is passed to this function. Also requires that the constants
 *          size and max are correctly set. 
 *    Post: The final data will be returned in a vector. The maximum value
 *          will be in the bottom right-most slot. 
 * Purpose: This function calculates the best value that can be held in the
 *          sack at one time using whole items. 
 */
int ** knapsack(std::vector<std::pair<int, int>> *pairs, const int size, const int max)
{
   // Columns will be the different weights,
   // Rows will be the weights (and values) of the items.
   int **sack = new int*[size];
   for(size_t i = 0; i < size; ++i)
      sack[i] = new int[max + 2];

   // For reference, put the weights in the first column.
   for(size_t i = 0; i < size; ++i) 
   {
      sack[i][0] = (*pairs)[i].first;
      sack[i][1] = 0;
   }

   // First row, starting at the third column (others are done)
   // The value will be zero until we reach the weight of the first item choice
   for(size_t i = 2; i < sack[0][0]; ++i)
      sack[0][i] = 0;
   
   for(size_t i = sack[0][0] + 1; i < max + 2; ++i)
      sack[0][i] = (*pairs)[0].second;
   
   // Second row and onward.
   for(size_t i = 1; i < size; ++i)
   {
      for(size_t j = 2; j < sack[i][0] + 2; ++j)
         sack[i][j] = sack[i - 1][j];

      for(size_t j = sack[i][0] + 1; j < max + 2; ++j)
         sack[i][j] = ((*pairs)[i].second + sack[i - 1][j - sack[i][0]]) > (sack[i - 1][j]) ?
                      ((*pairs)[i].second + sack[i - 1][j - sack[i][0]]) : (sack[i - 1][j]);
   }

   return sack;
}
