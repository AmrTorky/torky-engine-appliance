#!/usr/bin/env python3
"""
Torky Enterprise Fabric - Pre-GitHub Sync & Repository Verification Script
Author: Dr. Amr Torky
Purpose: Inspect git status, untracked evidence files, remote configurations, and repository sync health.
"""

import subprocess
import os

def run_cmd(command):
    try:
        result = subprocess.run(command, shell=True, check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        return result.stdout.strip()
    except subprocess.CalledProcessError as e:
        return f"ERROR: {e.stderr.strip()}"

def inspect_sync_state():
    print("============================================================")
    print("   PRE-GITHUB SYNC & REPOSITORY HEALTH INSPECTION           ")
    print("============================================================")
    
    # 1. Check current working directory
    cwd = os.getcwd()
    print(f"[1] Working Directory : {cwd}")
    
    # 2. Check git status
    git_status = run_cmd("git status -s")
    print(f"[2] Git Status Files  :\n{git_status if git_status else '    (Working tree clean / No changes)'}")
    
    # 3. Check active branch
    branch = run_cmd("git branch --show-current")
    print(f"[3] Active Git Branch : {branch if branch else 'Not a git repository or detached'}")
    
    # 4. Check remote repositories configuration
    remotes = run_cmd("git remote -v")
    print(f"[4] Remote Repositories:\n{remotes if remotes else '    (No remote origin configured yet)'}")
    
    # 5. List all local evidence reports generated in sandbox
    evidence_files = [f for f in os.listdir('.') if f.endswith('.json')]
    print(f"[5] Generated Evidence Artifacts ({len(evidence_files)} files):")
    for ef in evidence_files:
        print(f"    - {ef}")
        
    print("============================================================")
    print("         INSPECTION COMPLETED SUCCESSFULLY                  ")
    print("============================================================")

if __name__ == "__main__":
    inspect_sync_state()
