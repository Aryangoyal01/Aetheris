# AETHERIS — Coding Standards

This document defines the coding standards followed throughout the Aetheris codebase.

The goal is to produce code that is clean, maintainable, performant, and easy for both developers and AI agents to understand.

---

# Core Principles

Always prioritize:

* Readability over cleverness.
* Deterministic behavior.
* Explicit ownership.
* Modular design.
* Composition over inheritance.
* Long-term maintainability.

Every piece of code should have a clear purpose and a clear owner.

---

# Design Rules

Follow the Single Responsibility Principle.

Each:

* Class
* Struct
* Function
* File

should have one well-defined responsibility.

Avoid combining unrelated systems into a single component.

---

# Code Quality

Prefer:

* `constexpr` over macros.
* Explicit types when clarity improves readability.
* `enum class` instead of plain enums.
* `const` correctness whenever applicable.
* RAII for resource management.
* Early returns to reduce nesting.

Write code that is easy to understand before optimizing it.

---

# Architecture Rules

Never introduce:

* Global mutable state.
* Hidden ownership.
* Circular dependencies.
* Duplicate systems.
* Duplicate constants.
* Magic numbers.
* Temporary compatibility layers.

If a system becomes obsolete, remove it instead of maintaining parallel implementations.

---

# Project Organization

Keep files focused.

Each source file should implement one primary component.

Related functionality should be grouped into clearly defined modules.

Avoid unnecessary coupling between subsystems.

---

# Performance Guidelines

Optimize only where measurements justify it.

Prefer:

* Cache-friendly data layouts.
* Minimal allocations during runtime.
* Deterministic execution.
* Simple, predictable algorithms.

Avoid premature optimization that reduces readability.

---

# Documentation

Complex systems should explain **why** they exist rather than **what** the code does.

Documentation must be updated whenever architecture or major systems change.

---

# Development Requirements

Every checkpoint must:

* Compile successfully.
* Preserve existing functionality.
* Pass manual verification.
* Leave the project in a stable state.

No checkpoint is considered complete until it is buildable and reviewable.

---

# Guiding Philosophy

Aetheris is intended to remain a clean, professional, and long-lived codebase.

When faced with multiple implementation choices, prefer the solution that is simpler, clearer, and easier to maintain over the lifetime of the project.
