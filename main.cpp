#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

#define DICTIONARY_FILE_PATH "db.txt"
#define DB_CHUNK_SIZE (64 * 1024)
#define THREAD_COUNT 4
int line_number;

int levenstein_recursive(const std::string &str1, const std::string &str2,
                         int str1length, int str2length) {
  if (str1length == 0)
    return str2length;

  if (str2length == 0)
    return str1length;

  if (str1[str1length - 1] == str2[str2length - 1])
    return levenstein_recursive(str1, str2, str1length - 1, str2length - 1);

  return 1 + std::min(levenstein_recursive(str1, str2, str1length, str2length - 1),
                  std::min(levenstein_recursive(str1, str2, str1length - 1, str2length),
                           levenstein_recursive(str1, str2, str1length - 1, str2length - 1)));
}

// TODO: make this multithreaded
int levenstein(const std::string &str1, const std::string &str2) {
  int m = str1.length();
  int n = str2.length();

  std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));

  for (int i = 0; i <= m; i++)
    dp[i][0] = i;

  for (int j = 0; j <= n; j++)
    dp[0][j] = j;

  for (int i = 1; i <= m; i++)
    for (int j = 1; j <= n; j++) {
      if (str1[i - 1] == str2[j - 1])
        dp[i][j] = dp[i - 1][j - 1];
      else
        dp[i][j] = 1 + std::min(dp[i][j - 1], std::min(dp[i - 1][j], dp[i - 1][j - 1]));
    }

  return dp[m][n];
}

typedef struct {
  std::string word;
  int distance;
} word_distance_t;

std::vector<std::string> get_db_lines(const char c = '\0') {
  std::ifstream istr(DICTIONARY_FILE_PATH);

  if (!istr.is_open())
    throw std::runtime_error("couldn't open db file.");

  std::vector<std::string> lines;
  std::string line;

  while (std::getline(istr, line))
    if (c == line[0])
      lines.push_back(line);

  istr.close();

  return lines;
}

int main(int args, char **vargs) {
  if (args < 2)
    return EXIT_FAILURE;

  const std::string target(vargs[1]);

  std::vector<std::string> lines(get_db_lines(vargs[1][0]));

  std::vector<word_distance_t> out(lines.size());

  auto get_translation = [target](std::string &word) -> word_distance_t {
    std::string copy = word;
    std::string word_to_compare = strtok(word.data(), ":");
    return word_distance_t{copy, levenstein(target, word_to_compare)};
  };

  std::transform(lines.begin(), lines.end(), out.begin(), get_translation);

  auto get_closest_distance = [](const word_distance_t &a, const word_distance_t &b) -> bool {
    return a.distance < b.distance;
  };

  auto closest = *std::min_element(out.begin(), out.end(), get_closest_distance);

  std::cout << "Target: " << target << std::endl;
  std::cout << "Result: " << closest.word << " (distance: " << closest.distance << ")" << std::endl;

  return 0;
}
