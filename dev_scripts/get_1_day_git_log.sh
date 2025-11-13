#!/bin/bash

git log -p --since="$(date -d "1 days ago" --iso-8601)"
