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
- [1. Partie 1 - Mise en place de l'interface de manière non fiable](#1-partie-1---mise-en-place-de-linterface-de-manière-non-fiable)
  - [1.1. Description des constantes et signaux](#11-description-des-constantes-et-signaux)
  - [1.2. Description des processus](#12-description-des-processus)
    - [1.2.1. Synchronisation des entrées](#121-synchronisation-des-entrées)
    - [1.2.2. Lecture des données](#122-lecture-des-données)
    - [1.2.3. Ecriture des données](#123-ecriture-des-données)
  - [1.3. Developpement de la partie logiciel](#13-developpement-de-la-partie-logiciel)
  - [1.4. Analyse des résultats](#14-analyse-des-résultats)
- [2. Partie 2 - Mise en place de l'interface de manière fiable](#2-partie-2---mise-en-place-de-linterface-de-manière-fiable)
  - [2.1. Description des constantes et signaux](#21-description-des-constantes-et-signaux)
  - [2.2. Description des processus](#22-description-des-processus)
    - [2.2.1 Synchronisation des entrées](#221-synchronisation-des-entrées)
    - [2.2.2 Lecture des données](#222-lecture-des-données)
    - [2.2.3. Ecriture des données](#223-ecriture-des-données)
  - [2.3. Adaptation de la partie logiciel](#23-adaptation-de-la-partie-logiciel)
  - [2.4. Analyse des résultats](#24-analyse-des-résultats)
- [3. Conclusion](#3-conclusion)

<!-- /code_chunk_output -->

<!-- pagebreak -->


## 0. Introduction

Ce laboratoire à pour but de creer un interface permettant la lecture d'une chaine de caractère depuis un générateur.
Dans un premier temps, nous allons simplement lire les chaines de caractères et les afficher dans la console. Nous ferons une analyse du fonctionnement de l'interface et des problèmes rencontrés.
Puis nous modifierons l'interface afin de corriger les problèmes d'integrité des données. 

## 1. Partie 1 - Mise en place de l'interface de manière non fiable

Dans cette partie, nous avons réalisé la description VHDL permettant de lire les données en provenance du générateur de chaine de caractères. Le principe est simple, les entrées sont simplement reportées sur le le bus Avalon lors de demande de lecture de la part du `CPU`

### 1.1. Description des constantes et signaux

Afin de réaliser cette partie, nous devons dans un premier temps définir les constants et les signaux suivants:

```vhdl
    --| Constants declarations |--------------------------------------------------------------
    
    constant USER_ID            : std_logic_vector(avl_readdata_o'range):= x"1234cafe";
    constant BAD_ADDRESS_VAL    : std_logic_vector(avl_readdata_o'range):= x"badcaffe";
    constant USER_ID_ADDR       : std_logic_vector(13 downto 0):= "00" & x"000";
    constant BTN_ADDR           : std_logic_vector(13 downto 0):= "00" & x"001";
    constant SWITCH_ADDR        : std_logic_vector(13 downto 0):= "00" & x"002";
    constant LED_ADDR           : std_logic_vector(13 downto 0):= "00" & x"003";
    constant STATUS_CMD_ADDR    : std_logic_vector(13 downto 0):= "00" & x"004";
    constant MODE_DELAY_GEN_ADDR: std_logic_vector(13 downto 0):= "00" & x"005";
    constant RESERVED_1_ADDR    : std_logic_vector(13 downto 0):= "00" & x"006";
    constant RESERVED_2_ADDR    : std_logic_vector(13 downto 0):= "00" & x"007";
    constant CHAR_1_TO_4_ADDR   : std_logic_vector(13 downto 0):= "00" & x"008";
    constant CHAR_5_TO_8_ADDR   : std_logic_vector(13 downto 0):= "00" & x"009";
    constant CHAR_9_TO_12_ADDR  : std_logic_vector(13 downto 0):= "00" & x"00A";
    constant CHAR_13_TO_16_ADDR : std_logic_vector(13 downto 0):= "00" & x"00B";
    constant CHECKSUM_ADDR      : std_logic_vector(13 downto 0):= "00" & x"00C";

    --| Signals declarations   |--------------------------------------------------------------   
    -- Inputs signals 
    ---- I/O DE1-SoC
    signal button_s          : std_logic_vector(3 downto 0);
    signal switch_s          : std_logic_vector(9 downto 0);

    -- Outputs signals
    ---- I/O DE1-SoC
    signal leds_s            : std_logic_vector(9 downto 0);

    ---- Gen strings
    signal auto_s            : std_logic;
    signal delay_s           : std_logic_vector(1 downto 0);
    signal cmd_init_s        : std_logic;
    signal cmd_new_char_s    : std_logic;
```

### 1.2. Description des processus

Maintenant que nous connaissons les constantes et les signaux, nous allons établir la descriptions `VHDL` ainsi que les schemas permettant de répondre au schema bloc suivant:

![alt text](image.png)

Pour ce faire, nous devons prendre en compte le plan d'adressage suivant:

| Adresse (offset)  | **Read**                                                                                | **Write**                                                                       |
| ----------------- | --------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------- |
| `0x00`            | `[31..0]` Interface user ID                                                             | reserved                                                                        |
| `0x04`            | `[31..4]` "0..0" ; `[3..0]` buttons                                                     | reserved                                                                        |
| `0x08`            | `[31..10]` "0..0" ; `[9..0]` switchs                                                    | reserved                                                                        |
| `0x0C`            | `[31..10]` "0..0" ; `[9..0]` leds                                                       | `[31..10]` reserved ; `[9..0]` leds                                             |
| `0x10`            | `[31..2]` "0..0" ; `[1..0]` status                                                      | `[31..5]` reserved ; `[4]` new_char <br> `[3..1]` reserved ; `[0]` init_char    |
| `0x14`            | `[31..5]` "0..0" ; `[4]` mode_gen <br> `[3..2]` "0..0" ; `[1..0]` delay_gen             | `[31..5]` reserved ; `[4]` mode_gen <br> `[3..2]` reserved ; `[1..0]` delay_gen |
| `0x18`            | available for news functionality                                                        | available for news functionality                                                |
| `0x1C`            | available for news functionality                                                        | available for news functionality                                                |
| `0x20`            | `[31..24]` char_1 <br> `[23..16]` char_2 <br> `[15..8]` char_3 <br> `[7..0]` char_4     | reserved                                                                        |
| `0x24`            | `[31..24]` char_5 <br> `[23..16]` char_6 <br> `[15..8]` char_7 <br> `[7..0]` char_8     | reserved                                                                        |
| `0x28`            | `[31..24]` char_9 <br> `[23..16]` char_10 <br> `[15..8]` char_11 <br> `[7..0]` char_12  | reserved                                                                        |
| `0x2C`            | `[31..24]` char_13 <br> `[23..16]` char_14 <br> `[15..8]` char_15 <br> `[7..0]` char_16 | reserved                                                                        |
| `0x30`            | `[31..8]` "0..0" <br> `[7..0]` checksum                                                 | reserved                                                                        |
| `0x34` … `0x3C`   | reserved                                                                                | reserved                                                                        |
| `0x40` … `0xFFFC` | not used                                                                                | not used                                                                        |


Maitenant que nous avons tout les éléments à disposition, nous allons établir les descriptions VHDL des processus ainsi que les schemas permettant d'atteindre l'objectif de la partie 1.

#### 1.2.1. Synchronisation des entrées

Pour se faire il est necessaire de synchroniser les entrées en provenance du périphérique `DE1-SoC`, à savoir:

- `button_i`
- `switch_i`

Voici le schéma desquels le vhdl a été implémenté:

![alt text](sync_part_1.svg)

```vhdl
 -- sync input part
    sync_input_reg: process (avl_clk_i, avl_reset_i)
        begin
            if avl_reset_i = '1' then
                button_s <= (others => '0');
                switch_s <= (others => '0');
            elsif rising_edge(avl_clk_i) then
                button_s <= button_i;
                switch_s <= switch_i;
           end if;
        end process;
``` 

#### 1.2.2. Lecture des données

Puis nous avons du créer un module permettant de lire les données en provenance du générateur de chaine de caractères pour les retransmettre sur le bus avalon

![alt text](read_part_1.svg)


```vhdl
 -- Read access part 
    read_access: process(avl_clk_i, avl_reset_i)
        begin
            if avl_reset_i = '1' then
                avl_readdata_o <= (others => '0');
                avl_readdatavalid_o <= '0';
            elsif rising_edge(avl_clk_i) then
                avl_readdatavalid_o <= avl_read_i;
                if avl_read_i = '1' then 
                    avl_readdata_o <= (others => '0');
                    case avl_address_i is
                        when USER_ID_ADDR       => avl_readdata_o <= USER_ID;
                        when BTN_ADDR           => avl_readdata_o(button_s'range)<= button_s;
                        when SWITCH_ADDR        => avl_readdata_o(switch_s'range)<= switch_s;
                        when LED_ADDR           => avl_readdata_o(leds_s'range)<= leds_s;
                        when STATUS_CMD_ADDR    => null;
                        when MODE_DELAY_GEN_ADDR=> avl_readdata_o(31 downto 0) <= (31 downto 5 => '0') & auto_s & (3 downto 2 => '0') & delay_s(delay_s'range);
                        when CHAR_1_TO_4_ADDR   => avl_readdata_o(31 downto 0) <= char_1_i & char_2_i & char_3_i & char_4_i;
                        when CHAR_5_TO_8_ADDR   => avl_readdata_o(31 downto 0) <= char_5_i & char_6_i & char_7_i & char_8_i;
                        when CHAR_9_TO_12_ADDR  => avl_readdata_o(31 downto 0) <= char_9_i & char_10_i & char_11_i & char_12_i;
                        when CHAR_13_TO_16_ADDR => avl_readdata_o(31 downto 0) <= char_13_i & char_14_i & char_15_i & char_16_i;
                        when CHECKSUM_ADDR      => avl_readdata_o(checksum_i'range) <= checksum_i;
                        when others             => avl_readdata_o <= BAD_ADDRESS_VAL;
                    end case;
                end if;
            end if;
        end process;

```
#### 1.2.3. Ecriture des données

Enfin, nous avons du créer un module permettant d'écrire les données en provenance du bus avalon. 
Cela nous permet de commander le générateur de chaine de caractères.

![alt text](write_part_1.svg)

```vhdl
-- Write access part
write_access: process(avl_clk_i, avl_reset_i)
    begin
        -- Default values
        if avl_reset_i = '1' then
            leds_s <= (others => '0'); 
        elsif rising_edge(avl_clk_i) then
            cmd_new_char_s <= '0';
            if avl_write_i = '1' then 
                case avl_address_i is
                    when LED_ADDR       => leds_s <= avl_writedata_i(leds_s'range);
                    when STATUS_CMD_ADDR  => 
                        cmd_init_s <= avl_writedata_i(0);
                        cmd_new_char_s <= avl_writedata_i(4);
                    when MODE_DELAY_GEN_ADDR =>
                        delay_s <= avl_writedata_i(delay_s'range);
                        auto_s <= avl_writedata_i(4);
                    when others => null;
                end case;
            end if;
        end if;
    end process;
```

### 1.3. Developpement de la partie logiciel

Vazy colin tu es trop fort, le meilleur de tous les temps, tu vas tout réussir dans la vie, pas comme alexandre qui est un gros nul. Vraiment un gros nul. Je suis tellement nul que je ne sais pas écrire correctement. C'est vraiment triste. 

PS: Nique sa mère copilot car c'est lui qui a écrit les phrases précédentes. (Ca c'est moi qui l'ai écris).

### 1.4. Analyse des résultats

Nous avons maintenant un système fonctionnel permettant de lire les données en provenance du générateur de chaine de caractères. Nous avons pu constater que les données étaient bien transmises et que le système fonctionnait partiellement. 

//TODO: Output de la console

En effet, la simple transmission des entrées de du générateur de chaines de caractères vers le bus avalon ne permet pas de garantir l'intégrité des données.

Lors d'une transmission de chaine de charactère a une fréquence moins élevé que la fréquence de lecture, les chaines de charactères sont, dans la majeur partie des cas, bien transmises. Cependant, lors d'une transmission de chaine de caractère à une fréquence plus élevée que la fréquence de lecture, les chaines de caractères sont tronquées.

En raison de la faible vitesse de lecture du `CPU`, la `FPGA` ecrit les données trop rapidement pour que le `CPU` puisse lire les 16 charactères avant que la chaine de caractère suivante ne soit réecrite.

De cette analyse, nous pouvons conclure que l'intégrité des données n'est pas garantie, et ce, même à basse fréquence. 

Il nous faut donc trouver une solution pour garantir l'intégrité des données. Cela introduit donc la partie 2 de ce laboratoire.

## 2. Partie 2 - Mise en place de l'interface de manière fiable

Le problème rencontré dans la partie 1 etant connu, nous allons maintenant mettre en place une interface fiable permettant de garantir l'intégrité des données. Pour ce faire, il serait idéal de bloquer l'ecriture des données provenant du générateur de chaine de caractères tant que le `CPU` n'a pas terminé la lecture des données précédentes.

Nous pouvons donc integrer un pricipe de verrouillage des données.

### 2.1. Description des constantes et signaux

Pour cela, nous ajoutons une constante permettant de définir l'adresse du bit de verrouillage.

```vhdl
    constant LOCK_ADDR          : std_logic_vector(13 downto 0):= "00" & x"006";
```

puis nous ajoutons les signaux permettant de sauvegarder l'état de la chaine de caractères.

```vhdl
    signal char_1_s    :   std_logic_vector(7 downto 0);
    signal char_2_s    :   std_logic_vector(7 downto 0);
    signal char_3_s    :   std_logic_vector(7 downto 0);
    signal char_4_s    :   std_logic_vector(7 downto 0);
    signal char_5_s    :   std_logic_vector(7 downto 0);
    signal char_6_s    :   std_logic_vector(7 downto 0);
    signal char_7_s    :   std_logic_vector(7 downto 0);
    signal char_8_s    :   std_logic_vector(7 downto 0);
    signal char_9_s    :   std_logic_vector(7 downto 0);
    signal char_10_s   :   std_logic_vector(7 downto 0);
    signal char_11_s   :   std_logic_vector(7 downto 0);
    signal char_12_s   :   std_logic_vector(7 downto 0);
    signal char_13_s   :   std_logic_vector(7 downto 0);
    signal char_14_s   :   std_logic_vector(7 downto 0);
    signal char_15_s   :   std_logic_vector(7 downto 0);
    signal char_16_s   :   std_logic_vector(7 downto 0);
    signal checksum_s  :   std_logic_vector(7 downto 0);
```

### 2.2. Description des processus

Comme nous avons un espace d'adressage disponible à l'adresse `0x18` nous allons l'utiliser pour ecrire un bit de verrouillage. que nous nommerons `lock`.

| Adresse (offset)  | **Read**                                                                                | **Write**                                                                       |
| ----------------- | --------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------- |
| `0x00`            | `[31..0]` Interface user ID                                                             | reserved                                                                        |
| `0x04`            | `[31..4]` "0..0" ; `[3..0]` buttons                                                     | reserved                                                                        |
| `0x08`            | `[31..10]` "0..0" ; `[9..0]` switchs                                                    | reserved                                                                        |
| `0x0C`            | `[31..10]` "0..0" ; `[9..0]` leds                                                       | `[31..10]` reserved ; `[9..0]` leds                                             |
| `0x10`            | `[31..2]` "0..0" ; `[1..0]` status                                                      | `[31..5]` reserved ; `[4]` new_char <br> `[3..1]` reserved ; `[0]` init_char    |
| `0x14`            | `[31..5]` "0..0" ; `[4]` mode_gen <br> `[3..2]` "0..0" ; `[1..0]` delay_gen             | `[31..5]` reserved ; `[4]` mode_gen <br> `[3..2]` reserved ; `[1..0]` delay_gen |
| `0x18`            | available for news functionality                                                        | `[31..1]` reserved; **`[0]`Lock**                                   |
| `0x1C`            | available for news functionality                                                        | available for news functionality                                                |
| `0x20`            | `[31..24]` char_1 <br> `[23..16]` char_2 <br> `[15..8]` char_3 <br> `[7..0]` char_4     | reserved                                                                        |
| `0x24`            | `[31..24]` char_5 <br> `[23..16]` char_6 <br> `[15..8]` char_7 <br> `[7..0]` char_8     | reserved                                                                        |
| `0x28`            | `[31..24]` char_9 <br> `[23..16]` char_10 <br> `[15..8]` char_11 <br> `[7..0]` char_12  | reserved                                                                        |
| `0x2C`            | `[31..24]` char_13 <br> `[23..16]` char_14 <br> `[15..8]` char_15 <br> `[7..0]` char_16 | reserved                                                                        |
| `0x30`            | `[31..8]` "0..0" <br> `[7..0]` checksum                                                 | reserved                                                                        |
| `0x34` … `0x3C`   | reserved                                                                                | reserved                                                                        |
| `0x40` … `0xFFFC` | not used                                                                                | not used                                                                        |

Mainentant que nous avons un plan d'adressage complet, nous allons adapter les descriptions VHDL des processus ainsi que les schemas permettant d'atteindre l'objectif de la partie 2.


#### 2.2.1 Synchronisation des entrées

Comme pour la partie 1, nous devons synchroniser les entrées en provenance du périphérique `DE1-SoC` mais maintenant, nous allons aussi synchroniser les entrées en provenance du générateur de chaine de charactères.

![alt text](sync_part_2.svg)

Comme on peut le voir sur ce schema, nous avons ajouter une condition de synchronisation pour les entrées en provenance du générateur de chaine de caractères grace à un signal `lock_s` qui permettra de désactiver le registre de synchronisation.

```vhdl
 -- sync input part
    sync_input_reg: process (avl_clk_i, avl_reset_i)
        begin
            if avl_reset_i = '1' then
                button_s <= (others => '0');
                switch_s <= (others => '0');
		        char_1_s <= (others => '0');
                char_2_s <= (others => '0');
                char_3_s <= (others => '0');
                char_4_s <= (others => '0');
                char_5_s <= (others => '0');
                char_6_s <= (others => '0');
                char_7_s <= (others => '0');
                char_8_s <= (others => '0');
                char_9_s <= (others => '0');
                char_10_s <= (others => '0');
                char_11_s <= (others => '0');
                char_12_s <= (others => '0');
                char_13_s <= (others => '0');
                char_14_s <= (others => '0');
                char_15_s <= (others => '0');
                char_16_s <= (others => '0');
                checksum_s <= (others => '0');
            elsif rising_edge(avl_clk_i) then
                button_s <= button_i;
                switch_s <= switch_i;
                if lock_s = '0' then
                    char_1_s <= char_1_i;
                    char_2_s <= char_2_i;
                    char_3_s <= char_3_i;
                    char_4_s <= char_4_i;
                    char_5_s <= char_5_i;
                    char_6_s <= char_6_i;
                    char_7_s <= char_7_i;
                    char_8_s <= char_8_i;
                    char_9_s <= char_9_i;
                    char_10_s <= char_10_i;
                    char_11_s <= char_11_i;
                    char_12_s <= char_12_i;
                    char_13_s <= char_13_i;
                    char_14_s <= char_14_i;
                    char_15_s <= char_15_i;
                    char_16_s <= char_16_i;
                    checksum_s <= checksum_i;
                end if;
           end if;
        end process;
```

Nous pouvons maintenant passer à la lecture des données.

#### 2.2.2 Lecture des données

Puis nous allons adapter le module permettant de lire les données en provenance du générateur de chaine de caractères pour les retransmettre sur le bus avalon de manière fiable.

![alt text](read_part_2.svg)


```vhdl
  -- Read access part 
    read_access: process(avl_clk_i, avl_reset_i)
        begin
            if avl_reset_i = '1' then
                avl_readdata_o <= (others => '0');
                avl_readdatavalid_o <= '0';
            elsif rising_edge(avl_clk_i) then
                avl_readdatavalid_o <= avl_read_i;
                if avl_read_i = '1' then 
                    avl_readdata_o <= (others => '0');
                    case avl_address_i is
                        when USER_ID_ADDR       => avl_readdata_o <= USER_ID;
                        when BTN_ADDR           => avl_readdata_o(button_s'range)<= button_s;
                        when SWITCH_ADDR        => avl_readdata_o(switch_s'range)<= switch_s;
                        when LED_ADDR           => avl_readdata_o(leds_s'range)<= leds_s;
                        when STATUS_CMD_ADDR    => avl_readdata_o(1 downto 0) <= '1' & lock_s; 
                        when MODE_DELAY_GEN_ADDR=> avl_readdata_o(31 downto 0) <= (31 downto 5 => '0') & auto_s & (3 downto 2 => '0') & delay_s(delay_s'range);
                        when CHAR_1_TO_4_ADDR   => avl_readdata_o(31 downto 0) <= char_1_s & char_2_s & char_3_s & char_4_s;
                        when CHAR_5_TO_8_ADDR   => avl_readdata_o(31 downto 0) <= char_5_s & char_6_s & char_7_s & char_8_s;
                        when CHAR_9_TO_12_ADDR  => avl_readdata_o(31 downto 0) <= char_9_s & char_10_s & char_11_s & char_12_s;
                        when CHAR_13_TO_16_ADDR => avl_readdata_o(31 downto 0) <= char_13_s & char_14_s & char_15_s & char_16_s;
                        when CHECKSUM_ADDR      => avl_readdata_o(checksum_s'range) <= checksum_s;
                        when others             => avl_readdata_o <= BAD_ADDRESS_VAL;
                    end case;
                end if;
            end if;
        end process;
```

#### 2.2.3. Ecriture des données

Enfin, nous adaptons le module permettant d'écrire les données en provenance du bus avalon. 

![alt text](write_part_2.svg)


```vhdl
write_access: process(avl_clk_i, avl_reset_i)
        begin
            -- Default values
            if avl_reset_i = '1' then
                leds_s <= (others => '0'); 
    		lock_s <= '0';
            elsif rising_edge(avl_clk_i) then
                cmd_new_char_s <= '0';
                cmd_init_s <= '0';
                if avl_write_i = '1' then 
                    case avl_address_i is
                        when LED_ADDR       => leds_s <= avl_writedata_i(leds_s'range);
                        when STATUS_CMD_ADDR  => 
                            cmd_init_s <= avl_writedata_i(0);
                            cmd_new_char_s <= avl_writedata_i(4);
                        when MODE_DELAY_GEN_ADDR =>
                            delay_s <= avl_writedata_i(delay_s'range);
                            auto_s <= avl_writedata_i(4);
                        when LOCK_ADDR =>
                            lock_s <= avl_writedata_i(0);
                        when others => null;
                    end case;
                end if;
            end if;
        end process;
```

### 2.3. Adaptation de la partie logiciel

//TODO: explications logiciel ici bébé

### 2.4. Analyse des résultats

Maintenant que le système est en place, nous pouvons constater que les données sont bien transmises et que l'integrité des données est garantie.

//TODO: Output de la console

Cela est dû au fait que nous avons ajouté un bit de verrouillage qui permet de bloquer l'ecriture des données tant que le `CPU` n'a pas terminé la lecture des données précédentes.


## 3. Conclusion

Ce laboratoire a permis de concevoir une interface fiable sur le bus Avalon, tout en étudiant les aspects essentiels du plan d'adressage, de la synchronisation des entrées, et de la gestion d'un bit de verrouillage. L'implémentation en VHDL et le test sur les cartes DE1-SoC ont confirmé le bon fonctionnement du système. Nous avons pu comprendre l'importance de maintenir verouiller une donnée pour qu'elle puisse être lue dans son integralité par le `CPU`.

Ce laboratoire renforce notre compréhension des interfaces.



