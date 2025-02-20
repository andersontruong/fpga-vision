#!/bin/bash
# Run the training python script in the background and log output to train_output.txt
nohup python3 train_model.py > train_output.txt 2>&1 &
echo "Training has been started in the background. Check train_output.txt for logs."