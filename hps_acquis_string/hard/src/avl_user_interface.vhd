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
    
    constant USER_ID          : std_logic_vector(avl_readdata_o'range):= x"1234cafe";
    constant BAD_ADDRESS_VAL  : std_logic_vector(avl_readdata_o'range):= x"deadbeef";
    --| Signals declarations   |--------------------------------------------------------------   

begin
    
    -- Read access part
    
    -- Write access part
    
    -- Interface management
    
end rtl; 
