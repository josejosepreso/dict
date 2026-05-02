#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

int levenshteinRecursive(const std::string& str1, const std::string& str2, int str1length, int str2length)
{
  if (str1length == 0)
    return str2length;

  if (str2length == 0)
    return str1length;

  if (str1[str1length - 1] == str2[str2length - 1])
    return levenshteinRecursive(str1, str2, str1length - 1, str2length - 1);

  return 1 + std::min(levenshteinRecursive(str1, str2, str1length, str2length - 1),
                      std::min(levenshteinRecursive(str1, str2, str1length - 1, str2length),
                               levenshteinRecursive(str1, str2, str1length - 1, str2length - 1)));
}

typedef struct {
  std::string word;
  int distance;
} word_distance_t;

int main(int args, char **vargs)
{
  if (args < 2)
    return EXIT_FAILURE;

  const std::string target(vargs[1]);
  std::vector<std::string> words { "jose", "bautista", "roberto", "cubas" };
  std::vector<word_distance_t> out(words.size());

  auto calculate_distance = [target](std::string &word) -> word_distance_t {
    return word_distance_t {
      word,
      levenshteinRecursive(target, word, target.length(), word.length())
    };
  };

  std::transform(words.begin(), words.end(), out.begin(), calculate_distance);

  auto it = std::min_element(out.begin(), out.end(),[](const word_distance_t &a, const word_distance_t &b) -> bool {
    return a.distance < b.distance;
  });

  word_distance_t closest = *it;

  for (const word_distance_t& distance : out)
    std::cout << "(" << distance.distance << ", " << distance.word << ")" << std::endl;

  std::cout << "Target: " << target << std::endl << "Result: " << closest.word << " (distance: " << closest.distance << ")" << std::endl;

  return 0;
}

void dict(const std::string& target) {
}
