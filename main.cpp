#include <cstring>
#include <iostream>
#include <map>

#define DICTIONARY_FILE_PATH "db.txt"

int main(int argc, char *argv[])
{
  char *target = argv[1];

  if (target == NULL) {
    std::cout << "Usage: "
              << "\tdict <word>" << std::endl;
    return EXIT_FAILURE;
  }

  FILE *f = fopen(DICTIONARY_FILE_PATH, "r");

  if (f == NULL) {
    std::cout << "Couldn't open file " << DICTIONARY_FILE_PATH << std::endl;
    return EXIT_FAILURE;
  }

  char l[256];
  char *curr;
  char *curr_key;
  char *curr_val;

  std::map<std::string, std::string> db;

  while (fgets(l, sizeof l, f) != NULL) {
    printf("%s", l);
    curr_key = strtok(l, ":");
    curr_val = strtok(NULL, "");

    if (curr_key == NULL || curr_val == NULL)
      break;

    db.insert(std::pair{ std::string(curr_key), std::string(curr_val) });

    // if (strcmp(target, curr) != 0)
    //   continue;

    // std::cout << target << ":" << std::endl;

    // curr = strtok(NULL, ",");

    // while (curr != NULL) {
    //   std::cout << "\t" << curr;
    //   curr = strtok(NULL, ",");

    //   if (curr != NULL)
    //     std::cout << std::endl;
    // }

    // break;
  }

  return 0;
}
