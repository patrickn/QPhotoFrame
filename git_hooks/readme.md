
To enable Git hooks to be propagated when performing a clone, move the directory _.git/hooks_ (which is not tracked) to _./git\_hooks_ in the repo base directory (which is).

Each user must then enable this for a single repo by running:

```
git config core.hooksPath git_hooks
```

or globally for all repos, by running:

```
git config --global core.hooksPath git_hooks
```