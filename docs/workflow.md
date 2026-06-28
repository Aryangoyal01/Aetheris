# AETHERIS — Development Workflow

This document defines the standard development workflow for Aetheris.

Every feature, system, and architectural change must follow this process to ensure the project remains stable, maintainable, and easy to extend.

---

# Development Philosophy

Aetheris is developed through **small, incremental checkpoints**.

Each checkpoint should introduce one logical improvement while preserving the stability of the existing engine.

The objective is continuous progress without breaking previously completed systems.

---

# Checkpoint Rules

Every checkpoint must:

* Implement a single well-defined objective.
* Compile successfully.
* Preserve all existing functionality.
* Be independently testable.
* Stop immediately after the planned objective is complete.

Large multi-feature checkpoints should be avoided.

---

# Required Workflow

Every checkpoint follows the same sequence:

### 1. Planning

* Define the objective.
* Identify affected files.
* Confirm architectural impact.
* Keep the scope focused.

### 2. Implementation

* Make the smallest set of required changes.
* Follow the project coding standards.
* Avoid unrelated refactoring.

### 3. Build Verification

The project must compile successfully without introducing new warnings or errors.

### 4. Runtime Verification

Launch Aetheris and verify:

* The new feature behaves as expected.
* Existing functionality remains intact.
* No visual regressions or crashes occur.

### 5. Review

Review the implementation for:

* Code quality
* Performance
* Consistency with the architecture
* Maintainability

### 6. Approval

A checkpoint is considered complete only after manual approval.

Only then may development continue to the next checkpoint.

---

# Documentation Policy

Documentation is treated as part of the codebase.

Whenever a checkpoint introduces significant architectural, system, or workflow changes, the relevant documentation must be updated before the checkpoint is considered complete.

---

# Commit Philosophy

Each commit should represent one meaningful milestone.

A good commit should:

* Build successfully.
* Be reversible.
* Be understandable in isolation.
* Leave the project in a working state.

Avoid combining unrelated changes into a single commit.

---

# Long-Term Goal

By following this workflow, every milestone of Aetheris remains:

* Stable
* Testable
* Reviewable
* Well documented
* Easy for future contributors or AI agents to understand and extend
