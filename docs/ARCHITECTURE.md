# Architecture

The initial prototype is intentionally small:

- `src/main.cpp`: argument validation, filesystem queries, and console presentation.
- `CMakeLists.txt`: portable C++17 build configuration.
- `site/`: static product and documentation website.

## Planned modules

1. **Metadata provider** — normalize file size, extension, timestamps, and platform attributes.
2. **Directory scanner** — bounded, cancellable traversal with explicit permission-error handling.
3. **Analysis layer** — aggregate sizes and file-type distributions without changing source files.
4. **Presentation layer** — CLI output first; optional GUI later.

Keep filesystem access separate from presentation. Make long-running scans cancellable, avoid following symlinks by default, and never delete or rewrite user data as a side effect of inspection.
