#ifndef METADATA_H
#define METADATA_H

#include <map>
#include <string>

class Metadata {
private:
    std::map<std::string, std::string> data; 

public:
    std::map<std::string, std::string> getMetadata() const;

    void setMetadata(const std::map<std::string, std::string>& data);

    std::string getValue(const std::string& key) const;

    virtual void setValue(const std::string& key, const std::string& value);

    virtual ~Metadata() = default;
};

#endif // METADATA_H
