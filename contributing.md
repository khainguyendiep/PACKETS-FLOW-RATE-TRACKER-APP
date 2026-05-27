# Contributing Guide

## Branch Rules

Protected branches, DO NOT push directly to:
- `master`

## Correct Contribution Procedure

### Step 1 - Fork Repository
Click the fork button on GitHub to create a copy to your account.

### Step 2 -  Clone to your machine
```
git clone git@github.com:khainguyendiep/packets_flow_rate_tracker.git 
cd packets_flow_rate_tracker
```
### Step 3 - Create a new branch from develop
```
git checkout master
git pull origin master
git checkout -b feature/feature-name
```

#### Branch Naming Convention:
- feature/: add new feature
- fix/: fix bug
- docs/: update documentation
- refactor/: improve code

### Step 4 - Commit
```
git commit -m "[type](scope): brief description"
```
#### Type convention:
- feat/feature: add new feature
- refactor: improve code
- fix: fix bug
- docs: change documents
- style: change format of code
- chore: Modify minor files

#### Scope convention (optional):
Scope of module, file, component,...

### Step 5 - Push to your fork
```
git push origin feature/feature-name
```

### Step 6 - Open Pull Request
- Go to Github, open pull request from your branch.
- Clearly describe what you did and why.
- Wait for review and enjoy.
