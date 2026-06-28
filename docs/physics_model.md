# Physics Model

Current integration:

Semi-Implicit Euler

Update order:

velocity += acceleration * dt

position += velocity * dt

Current forces:

- gravity

Current collision modes:

- Bounce
- Wrap
- Destroy
- None

Future additions:

- constraints
- attractors
- fields
- fluids
- procedural forces

Physics must remain deterministic.