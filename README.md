# Secure Cloud

_Secure Cloud is an ultra-secure communication platform designed for Médecins Sans Frontières (MSF). It ensures complete confidentiality of communications, even in conflict zones, on older hardware and with low-bandwidth connections._

Available Languages
- French
- English
- Spanish

---

### 📌Table of Contents

- [Features](#✅features)
- [Architecture](#🧱architecture)
- [Requirements](#⚙️requirements)
- [Installation & Launch](#🚀installation--launch)
- [Usage](#⚒️usage)
- [Project Structure](#📁project-structure)
- [Translation](#🌐translation)
- [Routes](#🌍routes)
- [Tests](#🧪tests)
- [Team](#👥team)
- [Useful Links](#🔗useful-links)
- [License](#📄license)

---

### ✅Features

- End-to-end encrypted messaging (AES-256)
- Secure file sharing (streaming + compression)
- Strong authentication (JWT + MFA)
- Native Qt interface (Windows/Linux/macOS)
- High-performance C++ microservices architecture
- Automated CI/CD deployment (Docker)

---

### 🧱Architecture

Qt Client (UI)  
│  
API Gateway (JWT + Routing)  
├── auth-service  
├── messaging-service  
├── files-service  
├── audit-service  
└── deploy-service  

---

### ⚙️Requirements

- [Docker](https://www.docker.com/) & [Docker Compose](https://docs.docker.com/compose/)
- Qt 5 or 6
- CMake
- Git

---

### 🚀Installation & Launch

#### CMake

**Create the build directory**

- In CMake, select "GCC 14.2.0 x86_64-mingw32 (mingw32)"
- In the terminal, run the following commands:

```bash
mkdir build
cd build
cmake --build .
.\services\<>\messaging-service.exe
```

#### Docker


#### Qt

---

### ⚒️Usage
- Launch the compiled Qt interface
- Log in with an existing user
- Send encrypted messages or files
- Access logs via audit-service if authorized

---

### 📁Project Structure

securecloud/  
├── tests/  
├── docs/  
└── README.md  

#### 📐 Conventions

##### 🔀 Branch Naming

#### 🧾 Commit Naming

✨feat(auth): description to add
• text
• text

1. Gitmoji  
2. Type  
    feat: new feature  
    fix: bug fix  
    refactor: code refactoring (no behavior change)  
    docs: documentation update  
    style: formatting changes (indentation, spaces, etc.)  
    test: add/modify tests  
    chore: tasks with no direct functional impact (dependency updates, configs, etc.)  
3. Scope: auth/login/ui etc.  
4. Message  
    - Sentence starting with a lowercase letter  
    - No period at the end  
    - Use single quotes for terms (e.g., 'token', 'userId')  

---

### 🌐Translation

#### How to Translate

- Ensure that `lupdate ` and `lrelease` are available in our system `PATH`
```
$env:PATH="C:\Qt\6.7.1\mingw_64\bin;" + $env:PATH
```
- Check if the source file exists
```
dir client\src\main.cpp
```
- Extract all translation keys into the `.ts` files
```
lupdate client/src/main.cpp -ts common/i18n/es_ES.ts common/i18n/fr_FR.ts common/i18n/en_US.ts
```
- Compile the `.ts` files into `.qm` files
```
lrelease common/i18n/en_US.ts common/i18n/fr_FR.ts common/i18n/es_ES.ts
```
- compile traductions
```
cmake --build build --target client
```

---

### 🌍Routes

| Action | Route | Description |
|--------|-------|-------------|
| **GET** | `/` | Return Gateway Status |
| **GET** | `/auth/ping` | Return Authentification-service Status |
| **GET** | `/messaging/ping` | Return Messaging-service Status |
| **POST** | `/audit/services` | Logs services status into Audit database |
| **GET** | `/audit/services` | Fetch services status from Audit database |

---

### 🧪Tests

---

### 👥Team

- Lucas MARTINIE DE MAISONNEUVE

- Vanny LAMORTE

---

### 🔗Useful Links

#### [Lien vers QT](https://www.qt.io/)
#### [Lien vers QT](https://www.qt.io/)

---

### 📄License

All rights reserved © 2025 SecureCloud Team.

---


