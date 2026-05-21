# Lecture 12: Microservices Architecture

This lecture covers the transition from monoliths to microservices.

## The Network Boundary
When services talk over the network, everything changes.

> Note: Network calls are unreliable!

| Service | Responsibility |
| :--- | :--- |
| Auth | Identity |
| Order | Payments |

![Architecture](diagram.png)

* Service Discovery
* API Gateways
* Circuit Breakers

> Important: Resilience is key.

