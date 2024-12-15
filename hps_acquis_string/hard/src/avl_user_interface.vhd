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

use std.textio.all;
    
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
    constant BAD_ADDRESS_VAL    : std_logic_vector(avl_readdata_o'range):= x"badcaffe";
    constant USER_ID_ADDR       : std_logic_vector(13 downto 0):= "00" & x"000";
    constant BTN_ADDR           : std_logic_vector(13 downto 0):= "00" & x"001";
    constant SWITCH_ADDR        : std_logic_vector(13 downto 0):= "00" & x"002";
    constant LED_ADDR           : std_logic_vector(13 downto 0):= "00" & x"003";
    constant STATUS_CMD_ADDR    : std_logic_vector(13 downto 0):= "00" & x"004";
    constant MODE_DELAY_GEN_ADDR: std_logic_vector(13 downto 0):= "00" & x"005";
    constant LOCK_ADDR          : std_logic_vector(13 downto 0):= "00" & x"006";
    constant RESERVED           : std_logic_vector(13 downto 0):= "00" & x"007";
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
    signal auto_s            : std_logic;
    signal delay_s           : std_logic_vector(1 downto 0);
    signal cmd_init_s        : std_logic;
    signal cmd_new_char_s    : std_logic;
    signal lock_s        :   std_logic;

begin
    
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
        -- Write access part

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

    -- Output assignment
    led_o <= leds_s;
    avl_waitrequest_o <= '0';
    cmd_init_o <= cmd_init_s;
    cmd_new_char_o <= cmd_new_char_s;
    auto_o <= auto_s;
    delay_o <= delay_s;

end rtl; 
