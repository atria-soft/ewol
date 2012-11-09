#!/bin/bash

echo $* > Sources/libewol/tag
git tag $*
