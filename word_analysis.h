#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include "datasets.h"

struct WordStats{
    std::string word;
    long long totalUpvotes = 0;
    long long occurrences = 0;
    double averageUpvotes = 0.0;
};

namespace detail{
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

        std::unordered_map<std::string, std::pair<long long, long long>> wordData; // word -> {totalUpvotes, occurrences}

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
                auto &entry = wordData[word];
                entry.first += upvotes;
                entry.second += 1;
            }
        }

        std::vector<WordStats> stats;
        stats.reserve(wordData.size());

        for (const auto &item : wordData){
            const auto &word = item.first;
            long long totalUpvotes = item.second.first;
            long long occurrences = item.second.second;

            if (occurrences < static_cast<long long>(minOccurrences)){
                continue;
            }

            WordStats ws;
            ws.word = word;
            ws.totalUpvotes = totalUpvotes;
            ws.occurrences = occurrences;
            ws.averageUpvotes = static_cast<double>(totalUpvotes) / static_cast<double>(occurrences);
            stats.push_back(ws);
        }

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