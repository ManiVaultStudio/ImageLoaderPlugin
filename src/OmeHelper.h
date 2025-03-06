#pragma once

#include <string>
#include <vector>

// Just checks if the string starts with XML and contains an OME field
inline static bool validateXMLandOME(const std::string& xml) {
    // Check if the string begins with "<?xml"
    if (xml.size() < 5 || xml.substr(0, 5) != "<?xml") {
        return false;
    }

    // Check if it contains an "OME" tag
    if (xml.find("<OME") == std::string::npos) {
        return false;
    }

    return true;
}

// Assumes that each channel XML field starts with "Channel"
inline static std::vector<std::string> extractChannels(const std::string& xml) {
    std::vector<std::string> channels;

    // Method 1: Using string operations (more efficient for simple cases)
    size_t pos = 0;
    while (true) {
        // Find the start of a Channel tag
        size_t startPos = xml.find("<Channel", pos);
        if (startPos == std::string::npos) {
            break;
        }

        // Find the end of the Channel tag (looking for />)
        size_t endPos = xml.find("/>", startPos);
        if (endPos == std::string::npos) {
            break;
        }

        // Extract the complete channel tag
        std::string channelTag = xml.substr(startPos, endPos - startPos + 2);
        channels.push_back(channelTag);

        // Move past this channel to find the next one
        pos = endPos + 2;
    }

    return channels;
}

// Assumes that the name is stored in a "Name" field
inline static std::string extractChannelName(const std::string& channelEntry) {
    // Look for the Name attribute
    size_t namePos = channelEntry.find("Name=\"");
    if (namePos == std::string::npos) {
        return ""; // Name attribute not found
    }

    // Move position to the start of the name value (after Name=")
    namePos += 6; // 6 is the length of 'Name="'

    // Find the closing quote
    size_t nameEndPos = channelEntry.find("\"", namePos);
    if (nameEndPos == std::string::npos) {
        return ""; // Closing quote not found (malformed XML)
    }

    // Extract and return the name
    return channelEntry.substr(namePos, nameEndPos - namePos);
}
