#!/bin/bash

# How to use Pandoc
# To convert your existing HTML release notes into Markdown (useful to build your history), type the following command in your bash window:
# pandoc --from html --to markdown Release_Notes.html > Release_Notes.md
# To convert a .md file into an HTML one, type the following command in your bash window:
# pandoc -s -r markdown -t html5 -c "_htmresc/mini-st.css" Release_Notes.md > Release_Notes.html

RELEASE_NOTES_FILE='Release_Notes_MP25.md'
if [[ "$1" == "MP21"  ]]; then
    RELEASE_NOTES_FILE='Release_Notes_MP21.md'
fi


echo "$RELEASE_NOTES_FILE"

cp "$RELEASE_NOTES_FILE" Release_Notes.md

pandoc --self-contained -s -r markdown -t html5 -c "_htmresc/mini-st_2020.css" Release_Notes.md > Release_Notes.html

echo CMSIS_Device Release_Notes.html Updated
