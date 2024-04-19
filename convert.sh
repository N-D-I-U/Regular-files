#!/bin/bash

# Set the directory containing PDF files
PDF_DIR="./PDF"

# Create a directory to store converted text files
TXT_DIR="./txt"

# Iterate over each PDF file in the directory
for pdf_file in "$PDF_DIR"/*.pdf; do
    # Extract filename without extension
    filename=$(basename "$pdf_file" .pdf)

    # Convert PDF to text
    pdftotext "$pdf_file" "$TXT_DIR/$filename.txt"

    # Print confirmation message
    echo "Converted $pdf_file to $TXT_DIR/$filename.txt"
done

echo "Conversion complete."
