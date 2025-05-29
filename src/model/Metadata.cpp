#include "Metadata.hpp"

// Retrieve the entire metadata as a map of key-value pairs
std::map<std::string, std::string> Metadata::getMetadata() const {
    return data;
}

// Set new metadata by replacing the existing map with a new one
void Metadata::setMetadata(const std::map<std::string, std::string>& newData) {
    data = newData;
}

// Retrieve the value associated with a specific metadata key
std::string Metadata::getValue(const std::string& key) const {
    auto it = data.find(key); 
    if (it != data.end()) {
        return it->second; 
    }
    return ""; 
}

// Add or update a key-value pair in the metadata
void Metadata::setValue(const std::string& key, const std::string& value) {
    data[key] = value; 
}
