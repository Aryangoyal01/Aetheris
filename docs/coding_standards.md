# Coding Standards

Always prefer:

- readability
- deterministic behavior
- explicit ownership
- composition
- modularity

Avoid:

- globals
- hidden state
- duplicated constants
- duplicated systems
- circular dependencies
- magic numbers

One class.

One responsibility.

One owner.

Prefer constexpr over macros.

Prefer explicit types.

Delete obsolete code instead of preserving compatibility layers.

Every checkpoint must compile independently.