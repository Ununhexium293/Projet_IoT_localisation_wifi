#!/bin/bash

mkdir python_env
python -m venv python_env/
./python_env/bin/pip install uvicorn fastapi requests
