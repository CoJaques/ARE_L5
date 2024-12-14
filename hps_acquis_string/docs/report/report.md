<img src="logo.png" style="height:80px;">

# <center> Laboratoire n°05 {ignore=true}

# <center> Conception d’interface fiable {ignore=true}

## <center>Département : TIC {ignore=true}

## <center>unité d’enseignement ARE {ignore=true}

<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>

Auteur: **Colin Jaques & Alexandre Iorio**

Professeur: **Etienne Messerli**

Assistant : **Anthony Converse**

Salle de labo : **A07**

Date : **13.12.2024**



<!--pagebreak-->

## <center>Table des matières {ignore=true}


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [0. Introduction](#0-introduction)
- [1. Etude du bus Avalon](#1-etude-du-bus-avalon)
- [2. Définition du plan d'adressage](#2-définition-du-plan-dadressage)
  - [2.1. Plan d'adressage](#21-plan-dadressage)
  - [2.2. Description des adresses](#22-description-des-adresses)
- [3. Validation du plan d'adressage](#3-validation-du-plan-dadressage)
- [4. Schéma bloc de l'interface `Avalon`](#4-schéma-bloc-de-linterface-avalon)
  - [4.1. Syncronisation des entrées](#41-syncronisation-des-entrées)
  - [4.2. Décodeur d'adresse](#42-décodeur-dadresse)
  - [4.3. Gestion des registres](#43-gestion-des-registres)
  - [4.4. Gestion des sorties](#44-gestion-des-sorties)
- [5. Schéma bloc de l'interface parallèle `lp36`](#5-schéma-bloc-de-linterface-parallèle-lp36)
  - [5.1. Chronogramme de l'interface `lp36`](#51-chronogramme-de-linterface-lp36)
  - [5.2. Gestion du timer](#52-gestion-du-timer)
  - [5.3. Gestion des états](#53-gestion-des-états)
  - [5.4. Gestion des données du périphérique `lp36`](#54-gestion-des-données-du-périphérique-lp36)
- [6. Implémentation en `VHDL`](#6-implémentation-en-vhdl)
  - [6.1. Gestion des entrées](#61-gestion-des-entrées)
  - [6.2. Gestion de la lecture](#62-gestion-de-la-lecture)
  - [6.3. Gestion de l'écriture](#63-gestion-de-lécriture)
  - [6.4. Gestion de l'interface `lp36`](#64-gestion-de-linterface-lp36)
- [7. Simulation](#7-simulation)
- [8. Fonctionnement sur la carte `DE1-SoC` et la carte `MAX10_leds`](#8-fonctionnement-sur-la-carte-de1-soc-et-la-carte-max10_leds)
- [9. Implémentation du comportement en `C`](#9-implémentation-du-comportement-en-c)
  - [9.1. Interface en `C`](#91-interface-en-c)
  - [9.2. Application](#92-application)
- [10. Test complet du système](#10-test-complet-du-système)
- [11. Conclusion](#11-conclusion)

<!-- /code_chunk_output -->

<!-- pagebreak -->


## 0. Introduction


Ce laboratoire à pour but de creer un interface permettant la lecture d'une chaine de caractère depuis un générateur.
Dans un premier temps, nous allons simplement lire les chaines de caractères et les afficher dans la console. Nous ferons une analyse du fonctionnement de l'interface et des problèmes rencontrés.
Puis nous modifierons l'interface afin de corriger les problèmes d'integrité des données. 

## 1. Analyse du schema principal

