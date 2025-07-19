#ifndef __LOADER__
#define __LOADER__

#include <fstream>
#include <vector>
#include <utility>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

using Sample = std::vector<double>;
using Dataset = std::vector<std::pair<Sample, Sample>>;

Dataset loadDataset(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Impossible d'ouvrir le fichier : " + filename);
    }

    json data;
    file >> data;

    Dataset dataset;

    for (const auto& item : data) {
        Sample input, output;

        for (const auto& val : item["pixels"]) {
            input.push_back(static_cast<double>(val));
        }

        for (const auto& val : item["label_onehot"]) {
            output.push_back(val ? 1.0 : 0.0); // true -> 1.0, false -> 0.0
        }

        dataset.emplace_back(input, output);
    }

    return dataset;
}

Dataset loadDatasetJSON(const std::string& filename, std::size_t start, std::size_t count) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Impossible d'ouvrir le fichier : " + filename);
    }

    json data;
    file >> data;

    Dataset dataset;

    std::size_t index = 0;
    for (const auto& item : data) {
        if (index >= start + count) break;
        if (index >= start) {
            Sample input, output;

            for (const auto& val : item["pixels"]) {
                input.push_back(static_cast<double>(val));
            }

            for (const auto& val : item["label_onehot"]) {
                output.push_back(val ? 1.0 : 0.0);
            }

            dataset.emplace_back(input, output);
        }
        ++index;
    }

    return dataset;
}


Dataset loadDatasetJSONL(const std::string& filename, std::size_t start = 0, std::size_t count = 10) {
    std::ifstream file(filename);
    if (!file) throw std::runtime_error("Impossible d'ouvrir le fichier");

    Dataset dataset;
    std::string line;
    std::size_t index = 0;

    while (std::getline(file, line)) {
        if (index >= start + count) break;
        if (index >= start) {
            auto j = json::parse(line);
            Sample input, output;

            for (auto& val : j["pixels"])
                input.push_back(static_cast<double>(val));

            for (auto& val : j["label_onehot"])
                output.push_back(val ? 1.0 : 0.0);

            dataset.emplace_back(input, output);
        }
        ++index;
    }

    return dataset;
}


#endif