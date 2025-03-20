#!/bin/bash

SESSION_NAME="emugrid"
PROJECT_PATH="/home/emugrid/Project/emugrid.com"

tmux new-session -d -s $SESSION_NAME -c "$PROJECT_PATH"

# Window 1: nvim
tmux send-keys "nvim" C-m

tmux new-window -t $SESSION_NAME -n "lazygit" -c "$PROJECT_PATH"
tmux send-keys "lazygit" C-m

    # Window 3: Command Line 1 (same folder)
    tmux new-window -t $SESSION_NAME -n "cmd1" -c "$PROJECT_PATH"

    # Window 5: Core folder
    tmux new-window -t $SESSION_NAME -n "core" -c "$PROJECT_PATH/app/risc-v-emulator/core"

    # Select the first window (nvim)
    tmux select-window -t $SESSION_NAME:1

# Attach to the session
tmux attach-session -t $SESSION_NAME

