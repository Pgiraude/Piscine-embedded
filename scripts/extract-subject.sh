#!/bin/bash

# Script pour extraire le PDF du sujet en texte
# Usage: ./scripts/generate-readme.sh Module02
# ou: ./scripts/generate-readme.sh Rush00

if [ -z "$1" ]; then
    echo "Usage: $0 <ModuleName>"
    echo "Example: $0 Module02"
    echo "Example: $0 Rush00"
    exit 1
fi

MODULE_NAME="$1"
MODULE_PATH="$MODULE_NAME"

# Vérifier que le dossier existe
if [ ! -d "$MODULE_PATH" ]; then
    echo "Error: Directory $MODULE_PATH does not exist"
    exit 1
fi

# Vérifier que le PDF existe
if [ ! -f "$MODULE_PATH/fr.subject.pdf" ]; then
    echo "Error: $MODULE_PATH/fr.subject.pdf not found"
    exit 1
fi

# Extraire le PDF en texte
echo "Extracting PDF content from $MODULE_PATH/fr.subject.pdf..."
pdftotext "$MODULE_PATH/fr.subject.pdf" "$MODULE_PATH/subject.txt"

if [ $? -eq 0 ]; then
    echo "✓ PDF extracted to $MODULE_PATH/subject.txt"
    echo "✓ Ready for README generation"
else
    echo "✗ Error extracting PDF"
    exit 1
fi

