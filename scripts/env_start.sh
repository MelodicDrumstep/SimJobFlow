#!/bin/bash

if [ ! -d "third_party" ]; then
    echo "Error: Please run this script from the project root directory."
    exit 1
fi

cd third_party || exit

if [ ! -d "json" ]; then
    git clone https://github.com/nlohmann/json.git
else
    echo "nlohmann/json repository already exists. Skipping clone."
fi

cd ..