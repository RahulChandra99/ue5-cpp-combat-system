# Melee Combat Prototype

A third-person melee combat prototype inspired by **Elden Ring**, **Sekiro**, and **Ghost of Tsushima**.



This prototype focuses on:
- Satisfying melee gameplay
- Dynamic weapon-based combat
- Tight, responsive controls
- Challenging AI enemies
- Adaptive character animations based on equipped weapons

---

## 🎮 Current Gameplay
(Click on the Image)
[![Melee Combat Prototype](https://img.itch.zone/aW1hZ2UvMzUzNjM4Mi8yMTA2MTY5Ni5wbmc=/original/lzhhNG.png)](https://youtu.be/1wI59nvk9WY)


## Features
- Third-person melee combat system built in **Unreal Engine 5 (C++)**
- Multiple playable characters with unique weapons and abilities
- Light and heavy attacks, dodging, sprinting, and blocking mechanics
- Dynamic weapon equipping and unequipping system
- Early prototype includes basic enemy AI and environmental setup

---

## Prerequisites
- Unreal Engine 5.3 (or compatible UE5 version)
- Windows 10 or later (tested environment)

---

## Build Instructions
1. Clone the repository.
2. Open the project using **Unreal Engine 5.3**.
3. Build the project using the **Unreal Build Tool**.
4. Open the test map: `MeleeCombatPrototype/Maps/TestArena`.
5. Press **Play** in the editor.

---

## Entry Point & Project Structure

### Core Entry Point
- `AMeleeCombatGameMode` → Initializes player and world.
- `APlayerMeleeCharacter` → Handles all player combat and movement logic.

### Key Folders
- `/Characters` → Player and enemy characters.
- `/Weapons` → Weapon blueprints, data assets, and logic.
- `/Animations` → Animation blueprints and montages.
- `/Maps` → Test environments.

---

## Design Highlights
- **Modular Combat System**  
  Built using an extensible component-based architecture (`UCombatComponent`), allowing easy addition of new weapons, attacks, and abilities.

- **Data-Driven Weapons & Abilities**  
  Weapons are defined via **Data Assets**, enabling designers to tweak parameters without touching code.

- **State-Driven Animation System**  
  Uses **Animation Montages and State Machines** for weapon-specific attack combos and reactions.

---

## Controls (Prototype)

| Action        | Key / Mouse Button  |
|---------------|---------------------|
| Move          | WASD                |
| Roll / Sprint | Shift (Tap / Hold)  |
| Light Attack  | Left Mouse Button   |
| Heavy Attack  | Right Mouse Button  |
| Block         | Alt                 |
| Equip Weapon  | 1 / 2 / 3           |

---

## Current State
This is an **early prototype**.  
Planned features include:
- More characters, weapons, and abilities.
- Expanded enemy AI and boss fights.
- Polished animations and VFX.

---

## Follow the Development
Stay updated as the project grows.

