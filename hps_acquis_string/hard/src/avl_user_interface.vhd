------------------------------------------------------------------------------------------
-- HEIG-VD ///////////////////////////////////////////////////////////////////////////////
-- Haute Ecole d'Ingenerie et de Gestion du Canton de Vaud
-- School of Business and Engineering in Canton de Vaud
------------------------------------------------------------------------------------------
-- REDS Institute ////////////////////////////////////////////////////////////////////////
-- Reconfigurable Embedded Digital Systems
------------------------------------------------------------------------------------------
--
-- File                 : avl_user_interface.vhd
-- Author               : Anthony Convers
-- Date                 : 04.08.2022
--
-- Context              : Avalon user interface
--
------------------------------------------------------------------------------------------
-- Description : 
--   
------------------------------------------------------------------------------------------
-- Dependencies : 
--   
------------------------------------------------------------------------------------------
-- Modifications :
-- Ver    Date        Engineer    Comments
-- 0.0    See header              Initial version

------------------------------------------------------------------------------------------

library ieee;
    use ieee.std_logic_1164.all;
    use ieee.numeric_std.all;
    
entity avl_user_interface is
  port(
    -- Avalon bus
    avl_clk_i           : in  std_logic;
    avl_reset_i         : in  std_logic;
    avl_address_i       : in  std_logic_vector(13 downto 0);
    avl_byteenable_i    : in  std_logic_vector(3 downto 0);
    avl_write_i         : in  std_logic;
    avl_writedata_i     : in  std_logic_vector(31 downto 0);
    avl_read_i          : in  std_logic;
    avl_readdatavalid_o : out std_logic;
    avl_readdata_o      : out std_logic_vector(31 downto 0);
    avl_waitrequest_o   : out std_logic;
    -- User interface
    button_i            : in  std_logic_vector(3 downto 0);
    switch_i            : in  std_logic_vector(9 downto 0);
    led_o               : out std_logic_vector(9 downto 0);
    -- Gen strings
    char_1_i            : in  std_logic_vector(7 downto 0);
    char_2_i            : in  std_logic_vector(7 downto 0);
    char_3_i            : in  std_logic_vector(7 downto 0);
    char_4_i            : in  std_logic_vector(7 downto 0);
    char_5_i            : in  std_logic_vector(7 downto 0);
    char_6_i            : in  std_logic_vector(7 downto 0);
    char_7_i            : in  std_logic_vector(7 downto 0);
    char_8_i            : in  std_logic_vector(7 downto 0);
    char_9_i            : in  std_logic_vector(7 downto 0);
    char_10_i           : in  std_logic_vector(7 downto 0);
    char_11_i           : in  std_logic_vector(7 downto 0);
    char_12_i           : in  std_logic_vector(7 downto 0);
    char_13_i           : in  std_logic_vector(7 downto 0);
    char_14_i           : in  std_logic_vector(7 downto 0);
    char_15_i           : in  std_logic_vector(7 downto 0);
    char_16_i           : in  std_logic_vector(7 downto 0);
    checksum_i          : in  std_logic_vector(7 downto 0);
    cmd_init_o          : out std_logic;
    cmd_new_char_o      : out std_logic;
    auto_o              : out std_logic;
    delay_o             : out std_logic_vector(1 downto 0)
  );
end avl_user_interface;

architecture rtl of avl_user_interface is

    --| Components declaration |--------------------------------------------------------------
    
    --| Constants declarations |--------------------------------------------------------------
    
    constant USER_ID            : std_logic_vector(avl_readdata_o'range):= x"1234cafe";
    constant BAD_ADDRESS_VAL    : std_logic_vector(avl_readdata_o'range):= x"deadbeef";
    constant BTN_ADDR           : std_logic_vector(15 downto 0):= x"0004";
    constant SWITCH_ADDR        : std_logic_vector(15 downto 0):= x"0008";
    constant LED_ADDR           : std_logic_vector(15 downto 0):= x"000C";
    constant STATUS_ADDR        : std_logic_vector(15 downto 0):= x"0010";
    constant MODE_GEN_ADDR      : std_logic_vector(15 downto 0):= x"0014";
    constant DELAY_GEN_ADDR     : std_logic_vector(15 downto 0):= x"0014";
    constant CHAR_1_TO_4_ADDR   : std_logic_vector(15 downto 0):= x"0020";
    constant CHAR_5_TO_8_ADDR   : std_logic_vector(15 downto 0):= x"0024";
    constant CHAR_9_TO_12_ADDR  : std_logic_vector(15 downto 0):= x"0028";
    constant CHAR_13_TO_16_ADDR : std_logic_vector(15 downto 0):= x"002C";
    constant CHECKSUM_ADDR      : std_logic_vector(15 downto 0):= x"0030";
    --| Signals declarations   |--------------------------------------------------------------   
    -- Inputs signals 
    ---- I/O DE1-SoC
    signal button_s          : std_logic_vector(3 downto 0);
    signal switch_s          : std_logic_vector(9 downto 0);
    ---- Gen strings
    signal char_1_s          : std_logic_vector(7 downto 0);
    signal char_2_s          : std_logic_vector(7 downto 0);
    signal char_3_s          : std_logic_vector(7 downto 0);
    signal char_4_s          : std_logic_vector(7 downto 0);
    signal char_5_s          : std_logic_vector(7 downto 0);
    signal char_6_s          : std_logic_vector(7 downto 0);
    signal char_7_s          : std_logic_vector(7 downto 0);
    signal char_8_s          : std_logic_vector(7 downto 0);
    signal char_9_s          : std_logic_vector(7 downto 0);
    signal char_10_s         : std_logic_vector(7 downto 0);
    signal char_11_s         : std_logic_vector(7 downto 0);
    signal char_12_s         : std_logic_vector(7 downto 0);
    signal char_13_s         : std_logic_vector(7 downto 0);
    signal char_14_s         : std_logic_vector(7 downto 0);
    signal char_15_s         : std_logic_vector(7 downto 0);
    signal char_16_s         : std_logic_vector(7 downto 0);
    signal checksum_s        : std_logic_vector(7 downto 0);


    -- Outputs signals
    ---- I/O DE1-SoC
    signal leds_s            : std_logic_vector(9 downto 0);
    ---- Gen strings
    signal cmd_init_s        : std_logic;
    signal cmd_new_char_s    : std_logic;
    signal auto_s            : std_logic;
    signal delay_s           : std_logic_vector(1 downto 0);

begin
    
    -- Read access part

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
        end process;

    
    -- Write access part
    
    -- Interface management
    
end rtl; 
