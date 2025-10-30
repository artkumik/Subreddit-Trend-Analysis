#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <utility>
#include "datasets.h"

struct WordStats{
    std::string word;
    long long totalUpvotes = 0;
    long long occurrences = 0;
    double averageUpvotes = 0.0;
};

namespace detail{
    class WordHashTable{
    public:
        explicit WordHashTable(std::size_t initialCapacity = 1024)
            : buckets(nextPrime(initialCapacity)), count(0)
        {
        }

        void addWord(const std::string &word, long long upvotes){
            if (buckets.empty()){
                buckets.resize(1031);
            }

            if (needsRehash()){
                rehash(nextPrime(buckets.size() * 2));
            }

            std::size_t index = computeIndex(word);
            while (buckets[index].occupied){
                if (buckets[index].key == word){
                    buckets[index].totalUpvotes += upvotes;
                    buckets[index].occurrences += 1;
                    return;
                }
                index = (index + 1) % buckets.size();
            }

            buckets[index].occupied = true;
            buckets[index].key = word;
            buckets[index].totalUpvotes = upvotes;
            buckets[index].occurrences = 1;
            ++count;
        }

        template <typename Func>
        void forEach(Func func) const{
            for (const auto &bucket : buckets){
                if (bucket.occupied){
                    func(bucket.key, bucket.totalUpvotes, bucket.occurrences);
                }
            }
        }

    private:
        struct Bucket{
            bool occupied = false;
            std::string key;
            long long totalUpvotes = 0;
            long long occurrences = 0;
        };

        std::vector<Bucket> buckets;
        std::size_t count;

        static std::size_t nextPrime(std::size_t n){
            if (n <= 2){
                return 2;
            }

            if (n % 2 == 0){
                ++n;
            }

            while (!isPrime(n)){
                n += 2;
            }
            return n;
        }

        static bool isPrime(std::size_t n){
            if (n < 2){
                return false;
            }
            if (n % 2 == 0){
                return n == 2;
            }
            for (std::size_t i = 3; i * i <= n; i += 2){
                if (n % i == 0){
                    return false;
                }
            }
            return true;
        }

        bool needsRehash() const{
            if (buckets.empty()){
                return true;
            }
            return (count + 1) * 100 >= buckets.size() * 70;
        }

        void rehash(std::size_t newCapacity){
            std::vector<Bucket> oldBuckets = std::move(buckets);
            buckets.clear();
            buckets.resize(newCapacity);
            count = 0;

            for (const auto &bucket : oldBuckets){
                if (bucket.occupied){
                    insertDuringRehash(bucket);
                }
            }
        }

        void insertDuringRehash(const Bucket &bucket){
            std::size_t index = computeIndex(bucket.key);
            while (buckets[index].occupied){
                index = (index + 1) % buckets.size();
            }

            buckets[index].occupied = true;
            buckets[index].key = bucket.key;
            buckets[index].totalUpvotes = bucket.totalUpvotes;
            buckets[index].occurrences = bucket.occurrences;
            ++count;
        }

        std::size_t computeIndex(const std::string &word) const{
            std::size_t hashValue = 0;
            for (unsigned char ch : word){
                hashValue ^= ((hashValue << 5) + ch + (hashValue >> 2));
            }
            return hashValue % buckets.size();
        }
    };

    inline bool isValidWord(const std::string &word){
        if (word.empty()){
            return false;
        }

        return std::any_of(word.begin(), word.end(), [](unsigned char ch)
                           { return std::isalpha(ch); });
    }

    inline std::vector<std::string> splitWords(const std::string &text){
        std::vector<std::string> words;
        std::stringstream ss(text);
        std::string word;
        while (ss >> word){
            if (isValidWord(word)){
                words.push_back(word);
            }
        }
        return words;
    }

    inline std::vector<WordStats> computeWordStats(const std::string &path, std::size_t minOccurrences){
        std::ifstream file(path);
        if (!file.is_open()){
            std::cout << "Error opening file: " << path << std::endl;
            return {};
        }

        std::string line;

        // skip header
        if (!std::getline(file, line)){
            return {};
        }

        WordHashTable wordData;

        while (std::getline(file, line)){
            std::stringstream ss(line);
            std::string cell;
            std::vector<std::string> row_data;

            while (std::getline(ss, cell, ',')){
                row_data.push_back(cell);
            }

            if (row_data.size() < 4){
                continue;
            }

            if (row_data[0].empty() || !std::isdigit(static_cast<unsigned char>(row_data[0][0]))){
                continue;
            }

            long long upvotes = std::stoll(row_data[0]);
            std::string title = processString(row_data[2]);
            std::string body = processString(row_data[3]);

            auto words = splitWords(title + " " + body);

            for (const auto &word : words){
                wordData.addWord(word, upvotes);
            }
        }

        std::vector<WordStats> stats;
        wordData.forEach([&](const std::string &word, long long totalUpvotes, long long occurrences){
            if (occurrences < static_cast<long long>(minOccurrences)){
                return;
            }

            WordStats ws;
            ws.word = word;
            ws.totalUpvotes = totalUpvotes;
            ws.occurrences = occurrences;
            ws.averageUpvotes = static_cast<double>(totalUpvotes) / static_cast<double>(occurrences);
            stats.push_back(ws);
        });

        return stats;
    }
}

inline std::vector<WordStats> getTopWordsByAverageUpvotes(const std::string &path, std::size_t k, std::size_t minOccurrences = 3){
    auto stats = detail::computeWordStats(path, minOccurrences);
    std::sort(stats.begin(), stats.end(), [](const WordStats &a, const WordStats &b){
                  if (a.averageUpvotes == b.averageUpvotes){
                      if (a.occurrences == b.occurrences){
                          return a.word < b.word;
                      }
                      return a.occurrences > b.occurrences;
                  }
                  return a.averageUpvotes > b.averageUpvotes;
              });
    if (stats.size() > k){
        stats.resize(k);
    }
    return stats;
}

inline std::vector<WordStats> getBottomWordsByAverageUpvotes(const std::string &path, std::size_t k, std::size_t minOccurrences = 3){
    auto stats = detail::computeWordStats(path, minOccurrences);
    std::sort(stats.begin(), stats.end(), [](const WordStats &a, const WordStats &b){
                  if (a.averageUpvotes == b.averageUpvotes){
                      if (a.occurrences == b.occurrences){
                          return a.word < b.word;
                      }
                      return a.occurrences < b.occurrences;
                  }
                  return a.averageUpvotes < b.averageUpvotes;
              });
    if (stats.size() > k){
        stats.resize(k);
    }
    return stats;
}

inline void printWordStats(const std::vector<WordStats> &words){
    if (words.empty()){
        std::cout << "No words met the minimum occurrence threshold." << std::endl;
        return;
    }

    std::cout << std::fixed << std::setprecision(2);
    for (std::size_t i = 0; i < words.size(); ++i){
        const auto &ws = words[i];
        std::cout << (i + 1) << ". " << ws.word
                  << " | Avg Upvotes: " << ws.averageUpvotes
                  << " | Occurrences: " << ws.occurrences
                  << " | Total Upvotes: " << ws.totalUpvotes << std::endl;
    }
}
