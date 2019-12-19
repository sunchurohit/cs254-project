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

entity decrypter is
    Port ( clock : in  STD_LOGIC;
           K : in  STD_LOGIC_VECTOR (31 downto 0);
           C : in  STD_LOGIC_VECTOR (31 downto 0);
			  done:out STD_LOGIC;
           P : out  STD_LOGIC_VECTOR (31 downto 0);
           reset : in  STD_LOGIC;
           enable : in  STD_LOGIC);
end decrypter;

architecture Behavioral of decrypter is
signal T : std_logic_vector ( 3 downto 0) ;
signal tP : std_logic_vector (31 downto 0) ; -- for storing end value of each iteration
signal s1 , s2: bit  ; -- For initialisation
signal tN :  std_logic_vector( 5 downto 0) ; 
begin 
 
process(clock, reset, enable)
 begin
 	
 	if (reset = '1') then
	-- On reset. 
			P <= "00000000000000000000000000000000";
			s1 <= '0' ;
			s2 <= '0' ; 
			done <= '0';
	elsif (clock'event and clock = '1' and enable = '1' ) then
		-- Main for loop.
		if( s1 = '0') then
		--- Main initialisation
		s1 <= '1' ;
		-- make s2 active.
		s2 <= '1' ; 
		-- Initialise T, tP , tN. 
		T(3) <= K(31) xor K(27) xor K(23) xor K(19) xor K(15) xor K(11) xor K(7) xor K(3)  ;
		T(2) <= K(30) xor K(26) xor K(22) XOR K(18) xor K(14) xor K(10) xor K(6) xor K(2)  ;
		T(1) <= K(29) xor K(25) xor K(21) XOR K(17) xor K(13) xor K(9) xor K(5) xor K(1)  ;
		T(0) <= K(28) xor K(24) xor K(20) XOR K(16) xor K(12) xor K(8) xor K(4) xor K(0)  ;
		tP <= C ;
		-- tN will be N0. 
		tN <= "100000" - (("00000"&k(0))+("00000"&k(4))+("00000"&k(8))+("00000"&k(12))+
		("00000"&k(16))+("00000"&k(20))+("00000"&k(24))+("00000"&k(28))+
		("00000"&k(1))+("00000"&k(5))+("00000"&k(9))+("00000"&k(13))+("00000"&k(17))+("00000"&k(21))+("00000"&k(25))+("00000"&k(29))+
		("00000"&k(2))+("00000"&k(6))+("00000"&k(10))+("00000"&k(14))+
		("00000"&k(18))+("00000"&k(22))+("00000"&k(26))+("00000"&k(30))+
		("00000"&k(3))+("00000"&k(7))+("00000"&k(11))+("00000"&k(15))+
		("00000"&k(19))+("00000"&k(23))+("00000"&k(27))+("00000"&k(31)));
		end if ;
		if (s2 = '1') then 
		s2 <= '0' ; 
		T <= T + "1111" ; 
		end if ; 
		if (s1 = '1' and s2 =  '0' ) then
			if( tN = "000000") then
				-- after running N0 times.
					P <= tP ;
					done <= '1' ;
				else 
				-- else reassign tN, tP , T.
					tN <= tN + "111111" ; 
					tP <= tP xor (T & T & T & T & T & T & T & T )  ; 
					T <= T + "1111" ;	
			end if ; 
		end if ; 
			
		
	end if;	
			
end process;

end Behavioral;
