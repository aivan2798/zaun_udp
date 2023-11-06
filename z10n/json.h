std::map<std::string, std::string> decodeJsonString(const std::string& jsonString) {
    std::map<std::string, std::string> result;

    bool inKey = false;
    bool inValue = false;
    std::string currentKey;
    std::string currentValue;

    for (size_t i = 0; i < jsonString.length(); i++) {
        if (jsonString[i] == '{') {
            continue;
        } else if (jsonString[i] == '}') {
            if (inValue) {
                result[currentKey] = currentValue;
            }
            break;
        } else if (jsonString[i] == '"') {
            if (inKey) {
                inKey = false;
                inValue = true;
            } else {
                inKey = true;
                inValue = false;
            }
        } else if (jsonString[i] == ':') {
            inKey = false;
            inValue = true;
        } else if (jsonString[i] == ',') {
            result[currentKey] = currentValue;
            inKey = true;
            inValue = false;
            currentValue = "";
        } else if (inKey) {
            currentKey += jsonString[i];
        } else if (inValue) {
            currentValue += jsonString[i];
        }
    }

    return result;
}