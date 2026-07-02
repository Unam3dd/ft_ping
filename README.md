<div align="center">

# 🏓 ft_ping

### *Recoder `ping` — une plongée dans ICMP et les réseaux IPv4*

[![42](https://img.shields.io/badge/42-Project-000000?style=for-the-badge&logo=42&logoColor=white)](https://42.fr)
[![Language](https://img.shields.io/badge/Language-C-00599C?style=for-the-badge&logo=c&logoColor=white)](https://en.wikipedia.org/wiki/C_(programming_language))
[![RFC](https://img.shields.io/badge/RFC-792-blue?style=for-the-badge)](https://www.rfc-editor.org/rfc/rfc792)
[![Reference](https://img.shields.io/badge/Reference-inetutils--2.0-green?style=for-the-badge)](https://www.gnu.org/software/inetutils/)

> 🌐 Tester l'accessibilité d'une machine sur le réseau IP et mesurer le temps aller-retour (RTT).

</div>

---

## 📖 Table des matières

- [🎯 Objectif](#-objectif)
- [💡 Qu'est-ce que `ping` ?](#-quest-ce-que-ping-)
- [📡 ICMP & RFC 792](#-icmp--rfc-792)
- [🏗️ Exigences du projet](#️-exigences-du-projet)
- [⚙️ Options](#️-options)
- [🚀 Utilisation](#-utilisation)
- [🔧 Compilation](#-compilation)
- [📋 Critères d'évaluation](#-critères-dévaluation)
- [🎁 Partie bonus](#-partie-bonus)
- [📚 Références](#-références)

---

## 🎯 Objectif

**ft_ping** est un projet 42 qui consiste à **recoder la commande `ping`** en langage **C**, sans appeler le binaire système ni s'inspirer de ses sources.

L'objectif est de comprendre en profondeur :

- 🔹 Le protocole **ICMP** (Internet Control Message Protocol)
- 🔹 L'envoi et la réception de **paquets IPv4**
- 🔹 La mesure du **temps aller-retour** (Round-Trip Time)
- 🔹 La gestion des **erreurs réseau** et des options en ligne de commande

---

## 💡 Qu'est-ce que `ping` ?

`ping` est une commande qui permet de **tester l'accessibilité** d'une autre machine via le réseau IP. Elle envoie des paquets **ICMP Echo Request** et attend des **ICMP Echo Reply** en retour.

```
  ┌──────────┐                              ┌──────────┐
  │  Hôte A  │  ──── ICMP Echo Request ───► │  Hôte B  │
  │ (ft_ping)│  ◄─── ICMP Echo Reply  ───── │          │
  └──────────┘                              └──────────┘
         │                                          │
         └──────────── RTT = temps aller-retour ────┘
```

En pratique, `ping` affiche pour chaque réponse :

- 📦 Le nombre d'octets reçus
- 🏷️ L'adresse source de la réponse
- 🔢 Le numéro de séquence ICMP
- ⏱️ Le **TTL** (Time To Live) du paquet IP
- ⚡ Le **temps de réponse** en millisecondes

---

## 📡 ICMP & RFC 792

L'**ICMP** (*Internet Control Message Protocol*) est défini dans la **[RFC 792](https://www.rfc-editor.org/rfc/rfc792)** (septembre 1981, J. Postel, ISI). Il fait partie intégrante d'IP et **doit être implémenté par chaque module IP**.

### 🧩 Rôle d'ICMP

ICMP fournit un **retour d'information** sur les problèmes rencontrés lors du traitement des datagrammes IP :

| Type | Nom | Description |
|:----:|:---|:---|
| `0` | Echo Reply | Réponse à une requête echo |
| `3` | Destination Unreachable | Destination inaccessible |
| `4` | Source Quench | Congestion — ralentir l'émission |
| `5` | Redirect | Route plus courte disponible |
| `8` | **Echo** | **Requête echo (utilisée par `ping`)** |
| `11` | Time Exceeded | TTL expiré en transit |
| `12` | Parameter Problem | Erreur dans l'en-tête IP |
| `13` | Timestamp | Demande d'horodatage |
| `14` | Timestamp Reply | Réponse d'horodatage |

> ⚠️ Aucun message ICMP n'est envoyé **à propos** d'autres messages ICMP (évite la régression infinie).

### 🏓 Format du message Echo / Echo Reply

C'est le cœur de `ping`. Structure du paquet ICMP (RFC 792) :

```
 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|     Type      |     Code      |          Checksum             |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|           Identifier          |        Sequence Number        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|     Data ...
+-+-+-+-+-+-+-+-+
```

| Champ | Echo Request | Echo Reply |
|:---|:---:|:---:|
| **Type** | `8` | `0` |
| **Code** | `0` | `0` |
| **Checksum** | Complément à 1 sur 16 bits | idem |
| **Identifier** | Identifie la session (comme un port TCP/UDP) | renvoyé tel quel |
| **Sequence Number** | Incrémenté à chaque requête | renvoyé tel quel |
| **Data** | Données arbitraires | **doit être renvoyées à l'identique** |

### 🔐 Calcul du checksum ICMP

Le checksum est le **complément à 1 sur 16 bits** de la somme en complément à 1 de tous les mots de 16 bits du message ICMP, en commençant par le champ **Type**. Le champ checksum est mis à **zéro** pendant le calcul.

### 🌍 En-tête IP associé

Chaque message ICMP est encapsulé dans un datagramme IP avec notamment :

| Champ IP | Valeur ICMP |
|:---|:---|
| **Version** | `4` |
| **Protocol** | `1` (ICMP) |
| **TTL** | Décrémenté à chaque saut — expiré → ICMP Time Exceeded (type 11) |
| **Header Checksum** | Checksum de l'en-tête IP |

---

## 🏗️ Exigences du projet

### 📌 Règles générales

| Règle | Détail |
|:---|:---|
| 🗣️ Langage | **C** uniquement |
| 📦 Livrable | Un **Makefile** avec les règles habituelles |
| 🏷️ Binaire | Doit s'appeler **`ft_ping`** |
| 📚 libc | Toute la **libc** est autorisée |
| 🚫 Interdit | Appeler `ping` système ou utiliser ses sources |
| 🛡️ Robustesse | Aucun crash inattendu (segfault, bus error, double free…) |
| 📏 Référence | Comportement calqué sur **inetutils-2.0** (`ping -V`) |

### ✅ Partie obligatoire

- [ ] Exécutable nommé `ft_ping`
- [ ] Gestion des options **`-v`** et **`-?`**
- [ ] Prise en charge d'une adresse **IPv4** ou d'un **hostname** en argument
- [ ] Gestion des **FQDN** sans résolution DNS dans le paquet retourné
- [ ] Option **`-v`** : afficher les résultats en cas de problème ou d'erreur liée aux paquets (sans forcer l'arrêt du programme — modifier le TTL peut aider à provoquer une erreur)
- [ ] Famille **`printf`** autorisée

---

## ⚙️ Options

### 🔴 Obligatoires

| Option | Description |
|:---:|:---|
| `-v` | Mode verbeux — affiche les détails en cas d'erreur ou de problème de paquet |
| `-?` | Affiche l'aide |

### 🟡 Bonus (si la partie obligatoire est parfaite)

| Option | Description |
|:---:|:---|
| `-f` | Flood ping |
| `-l` | Précharge (preload) |
| `-n` | Pas de résolution DNS |
| `-w` | Délai d'attente (deadline) |
| `-W` | Timeout par réponse |
| `-p` | Pattern à envoyer dans les paquets |
| `-r` | Enregistrement de route |
| `-s` | Taille du paquet |
| `-T` | Options IP (TOS) |
| `--ttl` | Définir le TTL |
| `--ip-timestamp` | Option timestamp IP |

> ℹ️ Les options `-V`, `--usage` et `--echo` **ne comptent pas** comme bonus.  
> Deux flags pour la même fonctionnalité (ex. `-t` / `--type`) ne comptent qu'**une seule fois**.

---

## 🚀 Utilisation

```bash
# Ping basique vers une adresse IPv4
./ft_ping 8.8.8.8

# Ping vers un hostname
./ft_ping google.com

# Mode verbeux (affiche aussi les erreurs ICMP)
./ft_ping -v 127.0.0.1

# Aide
./ft_ping -?
```

### 📤 Exemple de sortie attendue (inetutils-2.0)

```
PING google.com (142.250.185.78) 56(84) bytes of data.
64 bytes from 142.250.185.78: icmp_seq=1 ttl=116 time=12.3 ms
64 bytes from 142.250.185.78: icmp_seq=2 ttl=116 time=11.8 ms
^C
--- google.com ping statistics ---
2 packets transmitted, 2 received, 0% packet loss, time 1001ms
rtt min/avg/max/mdev = 11.800/12.050/12.300/0.250 ms
```

---

## 🔧 Compilation

```bash
make        # Compile ft_ping
make clean  # Nettoie les fichiers objets
make fclean # Nettoie tout
make re     # Recompile entièrement
```

### 🖥️ Environnement requis

- 🐧 Machine virtuelle **Debian ≥ 7.0**
- 🐧 Noyau Linux **> 3.14**
- 🛠️ Outils de compilation C (`gcc`, `make`, …)

---

## 📋 Critères d'évaluation

| Critère | Tolérance |
|:---|:---|
| 📝 Indentation de la sortie | **Identique** à inetutils-2.0 (sauf ligne RTT et résolution DNS inverse) |
| ⏱️ Réception des paquets | Délai de **± 30 ms** accepté |
| 🧪 Partie bonus | Évaluée **uniquement** si la partie obligatoire est **parfaite** |
| 📂 Dépôt Git | Seul le contenu du dépôt est évalué lors de la soutenance |

---

## 🎁 Partie bonus

La partie bonus n'est évaluée que si **toute** la partie obligatoire fonctionne **sans aucun dysfonctionnement**. En cas de moindre défaut sur le mandatory, le bonus n'est **pas du tout** pris en compte.

---

## 📚 Références

| Document | Lien |
|:---|:---|
| 📄 RFC 792 — ICMP | [rfc-editor.org/rfc/rfc792](https://www.rfc-editor.org/rfc/rfc792) |
| 📄 RFC 791 — IP | [rfc-editor.org/rfc/rfc791](https://www.rfc-editor.org/rfc/rfc791) |
| 🔧 inetutils-2.0 (référence ping) | [gnu.org/software/inetutils](https://www.gnu.org/software/inetutils/) |
| 📋 Sujet 42 ft_ping | `subjects/en.subject.pdf` |

---

<div align="center">

### 🏓 *« Are you there? »* — *« Yes, in 12.3 ms. »*

*Projet réalisé dans le cadre du cursus **42***

</div>
