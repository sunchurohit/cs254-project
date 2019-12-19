---------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    05:12:16 01/23/2018 
-- Design Name: 
-- Module Name:    encrypter - Behavioral 
-- Project Name: 
-- Target Devices: 
-- Tool versions: 
-- Description: 
--
-- Dependencies: 
--
-- Revision: 
-- Revision 0.01 - File Created
-- Additional Comments: 
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.std_logic_unsigned.all ;
--use IEEE.numeric_std.all ;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity encrypter is
    Port ( clock : in  STD_LOGIC;
           K : in  STD_LOGIC_VECTOR (31 downto 0);
           P : in  STD_LOGIC_VECTOR (31 downto 0);
			  done:out STD_LOGIC;
           C : out  STD_LOGIC_VECTOR (31 downto 0);
           reset : in  STD_LOGIC;
           enable : in  STD_LOGIC);
end encrypter;

architecture Behavioral of encrypter is
signal T : std_logic_vector ( 3 downto 0) ; -- The 4 bit vector for Xor
signal tC : std_logic_vector (31 downto 0) ; -- The temporary signal for storing value 
								--at the end of every for loop iteration  
signal s : bit  ; -- for initialisation
 signal tN :  std_logic_vector( 5 downto 0) ; -- The signal for determining the number of for enable.
begin 
 
process(clock, reset, enable)
 begin
	-- Initialisation
	-- Incase of reset.
	
	if (reset = '1') then
			C <= "00000000000000000000000000000000";
			s <= '0' ; 
			done <= '0' ;
--- Main loop.
	elsif (enable = '1' and clock'event and clock = '1') then
		if(s = '0') then
			 s <= '1' ; 
		-- calculating T. 
		T(3) <= K(31) xor K(27) xor K(23) xor K(19) xor K(15) xor K(11) xor K(7) xor K(3)  ;
		T(2) <= K(30) xor K(26) xor K(22) XOR K(18) xor K(14) xor K(10) xor K(6) xor K(2)  ;
		T(1) <= K(29) xor K(25) xor K(21) XOR K(17) xor K(13) xor K(9) xor K(5) xor K(1)  ;
		T(0) <= K(28) xor K(24) xor K(20) XOR K(16) xor K(12) xor K(8) xor K(4) xor K(0)  ;
		-- Intialising tC. 
		tC <= P ;
		-- caluculating N1
		tN <= ("00000"&k(0))+("00000"&k(4))+("00000"&k(8))+("00000"&k(12))+
		("00000"&k(16))+("00000"&k(20))+("00000"&k(24))+("00000"&k(28))+
		("00000"&k(1))+("00000"&k(5))+("00000"&k(9))+("00000"&k(13))+("00000"&k(17))+("00000"&k(21))+("00000"&k(25))+("00000"&k(29))+
		("00000"&k(2))+("00000"&k(6))+("00000"&k(10))+("00000"&k(14))+
		("00000"&k(18))+("00000"&k(22))+("00000"&k(26))+("00000"&k(30))+
		("00000"&k(3))+("00000"&k(7))+("00000"&k(11))+("00000"&k(15))+
		("00000"&k(19))+("00000"&k(23))+("00000"&k(27))+("00000"&k(31));
		
		else
			if( tN = "000000") then
			C <= tC ; -- after tN times, assign C 
			done <= '1' ;
			else 
			tC <= tC xor ( T & T &T &T &T &T &T &T) ; -- Reassign tC, T, tN.  
			T <= T + "0001" ;			
			tN <= tN + "111111" ; 
			end if ;
		end if ;
	end if ;
	
end process;

end Behavioral;
