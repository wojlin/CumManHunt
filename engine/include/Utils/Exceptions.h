#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <iostream>
#include <exception>
#include <string>


class FileReadoutException : public std::exception {
public:
    FileReadoutException(const std::string& message) : errorMessage(message) {}

    const char* what() const noexcept override {
        return errorMessage.c_str(); // Use the correct member variable name here
    }

private:
    std::string errorMessage;
};


class WADCompilationException : public std::exception {
public:
    WADCompilationException(const std::string& message) : errorMessage(message) {}

    const char* what() const noexcept override {
        return errorMessage.c_str(); // Use the correct member variable name here
    }

private:
    std::string errorMessage;
};


class WADReadoutException : public std::exception {
public:
    WADReadoutException(const std::string& message) : errorMessage(message) {}

    const char* what() const noexcept override {
        return errorMessage.c_str(); // Use the correct member variable name here
    }

private:
    std::string errorMessage;
};


class LumpReadoutException : public std::exception {
public:
    LumpReadoutException(const std::string& message) : errorMessage(message) {}

    const char* what() const noexcept override {
        return errorMessage.c_str(); // Use the correct member variable name here
    }

private:
    std::string errorMessage;
};

#endif
