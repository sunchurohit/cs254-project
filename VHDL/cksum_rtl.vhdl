library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use IEEE.STD_LOGIC_UNSIGNED.all;

architecture rtl of swled is
	signal coordinates : std_logic_vector(31 downto 0) ;

	signal K,iniput,ciphertext_out: std_logic_vector(31 downto 0);
	signal ack_encrypt , cord_encrypt : std_logic_vector(31 downto 0) ; 
	signal encryption_over,doned,enable_enc,enable_dec,creset,resetd : std_logic ;
	signal encryption_over2 , creset2 : STD_LOGIC; 
	signal iniput2 : std_logic_vector(31 downto 0) ;
	signal ciphertext_out2 : std_logic_vector(31 downto 0) ; 
	signal count : integer ;
	signal count_if : integer ;
	signal ministate : std_logic_vector(2 downto 0);
	signal maindata : std_logic_vector(63 downto 0) ;
	signal center_now,left_now,up_now,down_now,right_now : STD_LOGIC;
	signal chan_r,chan_w: std_logic_vector(6 downto 0);
	signal datauc,dataready, led_done: std_logic ;
	signal uart_data: std_logic_vector(7 downto 0);
	signal send_data : std_logic_vector(7 downto 0) ; 
	signal count2 : std_logic_vector(1 downto 0) ;
	signal count3 : std_logic_vector(2 downto 0) ;
    signal stay :STD_LOGIC ;   
    signal host_data1 : std_logic_vector(31 downto 0) ; 
	signal host_data2 : std_logic_vector(31 downto 0) ;
	signal host_data3 : std_logic_vector(31 downto 0) ; 
	signal host_data4 : std_logic_vector(31 downto 0) ;
	signal data_out1	: std_logic_vector(31 downto 0) ; 
	signal data_out2	: std_logic_vector(31 downto 0) ;
    signal co1		       : std_logic_vector(7 downto 0) := (others => '0'); 
	signal co2                     :std_logic_vector(7 downto 0) := (others => '0');
	signal co3                     :std_logic_vector(7 downto 0) := (others => '0');
	signal co4                     :std_logic_vector(7 downto 0) := (others => '0');
	signal co5                     :std_logic_vector(7 downto 0) := (others => '0');
	signal co6                     :std_logic_vector(7 downto 0) := (others => '0');
	signal co7                     :std_logic_vector(7 downto 0) := (others => '0');
	signal co8                     :std_logic_vector(7 downto 0) := (others => '0');
	signal co11		       : std_logic_vector(7 downto 0) := (others => '0'); 
	signal co12                     :std_logic_vector(7 downto 0) := (others => '0');
	signal co13                     :std_logic_vector(7 downto 0) := (others => '0');
	signal co14                     :std_logic_vector(7 downto 0) := (others => '0');
	signal co15                     :std_logic_vector(7 downto 0) := (others => '0');
	signal co16                     :std_logic_vector(7 downto 0) := (others => '0');
	signal co17                     :std_logic_vector(7 downto 0) := (others => '0');
	signal co18                     :std_logic_vector(7 downto 0) := (others => '0');

	signal coord			:std_logic_vector(31 downto 0) := (others => '0'); 
	signal mainstate : std_logic_vector(4 downto 0) := "00000";
	signal done_dec1 : std_logic ; 
	signal done_dec2 : std_logic ;
	signal help : std_logic_vector( 2 downto 0) := "000"; 

component basic_uart is
generic (
  DIVISOR: natural
);
port (
  clk: in std_logic;   -- system clock
  reset: in std_logic;
  
  -- Client interface
  rx_data: out std_logic_vector(7 downto 0);  -- received byte
  rx_enable: out std_logic;  -- validates received byte (1 system clock spike)
  tx_data: in std_logic_vector(7 downto 0);  -- byte to send
  tx_enable: in std_logic;  -- validates byte to send if tx_ready is '1'
  tx_ready: out std_logic;  -- if '1', we can send a new byte, otherwise we won't take it
  
  -- Physical interface
  rx: in std_logic;
  tx: out std_logic
);
end component;

-- type fsm_mainstate_t is (idle, received, emitting);
-- type mainstate_t is
-- record
--   fsm_mainstate: fsm_mainstate_t; -- FSM mainstate
--   tx_data: std_logic_vector(7 downto 0);
--   tx_enable: std_logic;
-- end record;

signal uart_rx_data: std_logic_vector(7 downto 0);
signal ack_part : std_logic_vector(7 downto 0);
signal uart_rx_enable: std_logic;
signal uart_tx_data: std_logic_vector(7 downto 0);
signal uart_tx_enable: std_logic;
signal uart_tx_ready: std_logic;

-- signal mainstate_uart,mainstate_uart_next: mainstate_t;

begin
	basic_uart_inst: basic_uart
	generic map (DIVISOR => 1250) -- 2400
	port map (
		clk => clk_in, reset => center_now,
		rx_data => uart_rx_data, rx_enable => uart_rx_enable,
		tx_data => uart_tx_data, tx_enable => uart_tx_enable, tx_ready => uart_tx_ready,
		rx => uart_rx,
		tx => uart_tx
	);
	
	pmod_1 <= uart_tx_enable;
	pmod_2 <= uart_tx_ready;

	

	process(clk_in)
	variable counter :integer range 0 to 48000000;
	variable time : integer range 0 to 257 ; 
	begin
		if ( rising_edge(clk_in) ) then
			--count_if <= count_if + 1;
			if(uart_rx_enable = '1' and datauc = '0') then
				uart_data <= uart_rx_data;
				datauc <= '1';
			end if;
			-- Reset all the signals.
			if ( center_now = '1' ) then
				mainstate <= "11111";
				ministate <= "000"  ; 
				enable_enc <= '1' ;
				enable_dec <= '1' ;
				count <= 0 ; 
				--count_if <= 0;
				stay <= '0' ; 
				datauc <= '0' ; 
				creset <= '1' ; 
				count2 <= "00" ; 
		 		time := 0 ; 
		 		help <= "000" ; 
		 		
		 		co11 <= "11000111";
		 		co12 <= "11001111";
				co13 <= "11010111";
		 		co14 <= "11011111";
		 		co15 <= "11100111";
		 		co16 <= "11101111";
		 		co17 <= "11110111";
		 		co18 <= "11111111";		 		
		 		--ack_encrypt <= "11111111111111111111111111111111";
		 		--cord_encrypt <= "00000000000000000000000000000000";
			else
				end if;
			
					
	if(mainstate = "11111")then	--- send S1 macro state	
					led_out <= "11111111" ;
					stay <= '0' ; 
					help <= "000" ; 
					creset <= '1' ; 
					creset2 <= '1' ; 
					datauc <= '0' ; 
					counter := counter + 1 ;
					if(counter = 5) then
					time := time + 1;
					end if;
					
					if( time = 0 ) then
					led_out <= "11111111";
					elsif( time = 3 ) then
 
					led_out <= "00000000";				
					counter := 0;
					time := 0;
					mainstate <= "10000";		
					end if; 	
	elsif (mainstate = "10000") then -- fill ack1 , coord in this time.
			led_out <= "11110000" ;  
			if (help = "000") then
						iniput <= "00000000000000000000000000100010" ; 
						help <= "001" ; 
				elsif help = "001" then
						creset <= '0' ; 
						help <= "010" ; 
				elsif help = "010" and encryption_over = '1' then 
						cord_encrypt <= ciphertext_out ; 
						creset <= '1' ;
						help <= "011" ; 
				elsif help = "011" then
						led_out <= "10101010"; 
						iniput <= "11111111111111111111111111111111" ; 
						help <= "100" ; 
				elsif help = "100" then
						creset <= '0' ;
						help <= "101" ;  
				elsif help = "101" and encryption_over = '1' then
						ack_encrypt <= ciphertext_out ; 
						help <= "000" ;
						mainstate <= "00001" ; 
						creset <= '1' ;  
			end if ; 
	------------S2 Macro state.						
	elsif(mainstate = "00001")then --  start of macro state 2	
		if chanAddr_in = chan_w and f2hReady_in = '1' then 
			--led_out <= "00001111" ; 			
			
			if(count2 = "00") then
										
				send_data <= cord_encrypt(31 downto 24);
				count2 <= "01";
			elsif(count2 = "01") then
				
				send_data <= cord_encrypt(23 downto 16);
				count2 <= "10";
			elsif(count2 = "10") then
				
				send_data <= cord_encrypt(15 downto 8);
				count2 <= "11";
			elsif(count2 = "11") then
				time := 0 ; 
				counter := 0 ; 
				send_data <= cord_encrypt(7 downto 0);
				count2 <= "00" ; 
				count3 <= "000" ;
				mainstate <= "00010" ;
				creset <= '1' ; 
			
			end if;
			end if;
	--- get co ordinates again
	elsif mainstate = "00010" then
			led_out <= "00000011" ; 
		
			counter := counter+1;
			if counter = 5 then
					time := time + 1; 
			end if ; 	
			if(time = 256) then
			
			mainstate <= "00001" ;
			
			else 
			if chanAddr_in = chan_r and h2fValid_in = '1' then 
			led_out <= "10101010" ; 
			if(count3 = "000") then
				count3 <= "001" ; 
			--led_out <= h2fData_in ;
				host_data1(31 downto 24) <= h2fData_in ;
			elsif(count3 = "001") then
				count3 <= "010" ;
			
				host_data1(23 downto 16) <= h2fData_in ; 
			--led_out <= host_data1(23 downto 16) ;		
			elsif(count3 = "010") then
				count3 <= "011" ; 
			--led_out <= h2fData_in ;		
				host_data1(15 downto 8) <= h2fData_in ; 
			elsif(count3 = "011") then
				count3 <= "000" ;
			--led_out <= h2fData_in ; 
				host_data1(7 downto 0) <= h2fData_in ; 
				mainstate <= "00011" ; 
			end if ;
			end if;
			end if ;		 	 
			--end if ; 
	------- confirming ack state (= co ordinates).
	elsif(mainstate = "00011" ) then 
		led_out <= "00001111" ; 
		 
		creset <= '0' ; 
		
	if done_dec1 = '1' then
		 
		creset <= '1' ;
		counter := 0 ;  
		if data_out1 = coord then
		mainstate <= "00100" ; 
		creset <= '0' ; 
		led_out <= "00000001";
		
		end if ; 
		end if;  
	--------- send ack1
	elsif( mainstate = "00100") then
	
		led_out <= "11000011";			
			if chanAddr_in = chan_w and f2hReady_in = '1' then 
			--led_out <= "00001111" ; 			
			
			if(count2 = "00") then
										
				send_data <= ack_encrypt(31 downto 24);
				count2 <= "01";
			elsif(count2 = "01") then
				
				send_data <= ack_encrypt(23 downto 16);
				count2 <= "10";
			elsif(count2 = "10") then
				
				send_data <= ack_encrypt(15 downto 8);
				count2 <= "11";
			elsif(count2 = "11") then
				time := 0 ; 
				counter := 0 ; 
				send_data <= ack_encrypt(7 downto 0);
				count2 <= "00" ; 
				count3 <= "000" ;
				mainstate <= "00101" ;
				creset <= '1' ;
		end if;
		end if;
	----- receiving ack2
	elsif mainstate = "00101"  then  
		led_out <= "00001110" ; 		
		counter := counter+1;
		if counter = 5 then
				time := time + 1; 
		end if ; 	
		if(time = 256) then
		time := 0 ; 
		counter := 0 ; 
		mainstate <= "00001" ;
		
		else 
		if chanAddr_in = chan_r and h2fValid_in = '1' then 
		if(count3 = "000") then
		count3 <= count3 + "001" ; 
		--led_out <= h2fData_in ;
		host_data1(31 downto 24) <= h2fData_in ;
		
		end if ;
		if(count3 = "001") then
		count3 <= count3 + "001" ;
		
		host_data1(23 downto 16) <= h2fData_in ; 
		--led_out <= host_data1(23 downto 16) ;		
		end if ;
		if(count3 = "010") then
		count3 <= count3 + "001" ; 
		--led_out <= h2fData_in ;		
		host_data1(15 downto 8) <= h2fData_in ; 
		end if ;
		if(count3 = "011") then
		count3 <= "000" ;
		--led_out <= h2fData_in ; 
		host_data1(7 downto 0) <= h2fData_in ; 
		mainstate <= "00110" ;
		time := 0 ; 
		counter := 0 ; 
		end if ;
		end if;
		end if;
		---- receiving host data
	elsif chanAddr_in = chan_r and h2fValid_in = '1' and mainstate = "00110" then 
		led_out <= "10000001" ; 		
		if(count3 = "000") then
		count3 <= "001" ; 
		--led_out <= h2fData_in ;
		host_data3(31 downto 24) <= h2fData_in ;
		end if ;
		if(count3 = "001") then
		count3 <= "010" ;
		
		host_data3(23 downto 16) <= h2fData_in ; 
		--led_out <= host_data1(23 downto 16) ;		
		end if ;
		if(count3 = "010") then
		count3 <= "011" ; 
		--led_out <= h2fData_in ;		
		host_data3(15 downto 8) <= h2fData_in ; 
		end if ;
		if(count3 = "011") then
		count3 <= "100" ;
		--led_out <= h2fData_in ; 
		host_data3(7 downto 0) <= h2fData_in ; 
		mainstate <= "00111" ; 
		end if ;
	--------- send ack1 
	elsif(mainstate = "00111") then
		creset <= '0' ; 
		led_out <= "00000111" ;  
		if chanAddr_in = chan_w and f2hReady_in = '1' then 
	 		
		if(count2 = "00") then
			
			send_data <= ack_encrypt(31 downto 24);
			count2 <= "01";
		elsif(count2 = "01") then
			
			send_data <= ack_encrypt(23 downto 16);
			count2 <= "10";
		elsif(count2 = "10") then
			
			send_data <= ack_encrypt(15 downto 8);
			count2 <= "11";
		elsif(count2 = "11") then
			
			send_data <= ack_encrypt(7 downto 0);
			count2 <= "00";
			mainstate <= "01000" ;
		end if;
		end if;

		--- compu send the next 4 bytes
		elsif chanAddr_in = chan_r and h2fValid_in = '1' and mainstate = "01000" then
			led_out <= "00001000" ;  
			if(count3 = "100") then
			count3 <= count3 + "001" ; 
			--led_out <= h2fData_in ;
			host_data4(31 downto 24) <= h2fData_in ; 
			elsif(count3 = "101") then
			count3 <= count3 + "001" ; 
			--led_out <= h2fData_in ;
			host_data4(23 downto 16) <= h2fData_in ; 
			elsif(count3 = "110") then
			count3 <= count3 + "001" ; 
			--led_out <= h2fData_in ;
			host_data4(15 downto 8) <= h2fData_in ; 
			elsif(count3 = "111") then
			count3 <= "000" ;  
			mainstate <= "01001" ; 
			--led_out <= h2fData_in ;
			host_data4(7 downto 0) <= h2fData_in ; 
			end if ;
			--end if;
		---- state 9 - send ack1 again 
		elsif(mainstate = "01001") then
			led_out <= "00001001" ; 
					if(count2 = "00") then
						send_data <= ack_encrypt(31 downto 24);
						count2 <= "01";
					elsif(count2 = "01") then
						
						send_data <= ack_encrypt(23 downto 16);
						count2 <= "10";
					elsif(count2 = "10") then
						
						send_data <= ack_encrypt(15 downto 8);
						count2 <= "11";
					elsif(count2 = "11") then
						
						send_data <= ack_encrypt(7 downto 0);
						count2 <= "00";
						mainstate <= "01010" ;
						creset <= '1' ; 
					end if ; 
--- state 10 recevie ack2
	elsif mainstate = "01010"  then 
		led_out <= "00001010" ;  
		counter := counter+1;
		if counter = 5 then
				time := time + 1; 
		end if ; 	
		if(time = 256) then
			mainstate <= "00000" ;
		else 	
			if chanAddr_in = chan_r and h2fValid_in = '1' then 
				if(count3 = "000") then
					count3 <= count3 + "001" ; 
					--led_out <= h2fData_in ;
					host_data1(31 downto 24) <= h2fData_in ;
				
				end if ;
				if(count3 = "001") then
					count3 <= count3 + "001" ;
					
					host_data1(23 downto 16) <= h2fData_in ; 
				--led_out <= host_data1(23 downto 16) ;		
				end if ;
				if(count3 = "010") then
					count3 <= count3 + "001" ; 
					--led_out <= h2fData_in ;		
					host_data1(15 downto 8) <= h2fData_in ; 
				end if ;
				if(count3 = "011") then
					count3 <= count3 + "001" ;
					--led_out <= h2fData_in ; 
					host_data1(7 downto 0) <= h2fData_in ; 
					time := 0 ; 
					counter := 0 ; 
					mainstate <= "01011" ; 
				end if ;
			end if ; 
		end if ; 
		
	--- state 11 confirm ack  
	elsif(mainstate = "01011" ) then
		led_out <= "00001011" ;  
		enable_dec <= '1' ; 
		creset <= '0' ; 
		
		if done_dec1 = '1' then
			enable_dec <= '0' ; 
			creset <= '1' ;
			counter := 0 ;  
			if data_out1 = "11111111111111111111111111111111" then
				mainstate <= "01110" ;
				time := 0 ; 
				counter := 0 ;
				host_data1 <= host_data3 ; 
				host_data2 <= host_data4 ; 
			else	
				mainstate <= "00001";
				time := 0 ; 
				counter := 0 ;	
				host_data1 <= host_data3 ; 
				host_data2 <= host_data4 ; 	 
			end if ; 
		end if;  		
		---- load data 
	elsif(mainstate = "01110" ) then
				led_out <= "11100111" ;  
				enable_dec <= '1' ; 
				creset <= '0' ; 

			if done_dec1 = '1' and done_dec2 = '1' then
				mainstate <= "11101" ;
				time := 0 ; 
				counter := 0 ; 
				enable_dec <= '0' ; 
				creset <= '1' ;
				counter := 0 ;  
				--led_out <= host_data1(23 downto 16) ;				
					
					co1 <= data_out1(31 downto 24) ;
					co2 <= data_out1(23 downto 16) ;
					co3 <= data_out1(15 downto 8) ;
					co4 <= data_out1(7 downto 0) ;
					co5 <= data_out2(31 downto 24) ;
					co6 <= data_out2(23 downto 16) ;
					co7 <= data_out2(15 downto 8) ;
					co8 <= data_out2(7 downto 0) ;	
				
				--------------------------------
			end if;  
	---- updating data with local data.
	elsif mainstate = "11101" then 
			co1(6 downto 0 ) <= co11(6 downto 0) and co1(6 downto 0) ; 
			co2(6 downto 0 ) <= co12(6 downto 0) and co2(6 downto 0) ; 
			co3(6 downto 0 ) <= co13(6 downto 0) and co3(6 downto 0) ; 
			co4(6 downto 0 ) <= co14(6 downto 0) and co4(6 downto 0) ; 
			co5(6 downto 0 ) <= co15(6 downto 0) and co5(6 downto 0) ; 
			co6(6 downto 0 ) <= co16(6 downto 0) and co6(6 downto 0) ; 
			co7(6 downto 0 ) <= co17(6 downto 0) and co7(6 downto 0) ; 
			co8(6 downto 0 ) <= co18(6 downto 0) and co8(6 downto 0) ; 
			mainstate <= "01111" ; 
	-- displaying data
	elsif mainstate = "01111"  then  
					counter := counter+1;
					if counter = 5 then
							time := time + 1; 
					end if ; 		
							--------------------------------------------------
						-----scary if conditions..
						if (time = 1) then
								if(co1(7 downto 6) /= "11") then
									--if(co1(7) = '0') then
										led_out(2 downto 0) <= "001";
										led_out(7 downto 5) <= co1(5 downto 3);
			
									else	 
										led_out(7 downto 5) <= co1(5 downto 3);
										if(sw_in(0) = '1' and sw_in(4) = '0') then
											if(co1(2 downto 0) = "001") then
												led_out(2 downto 0) <= "100";
											else
												led_out(2 downto 0) <= "010";
											end if;
										else
											led_out(2 downto 0) <= "001";
										end if;
								
								end if;
								-------------------------------
						elsif (time = 4) then
								if(co2(7 downto 6) /= "11") then
											led_out(2 downto 0) <= "001";
											led_out(7 downto 5) <= co2(5 downto 3);			
								else	 
										led_out(7 downto 5) <= co2(5 downto 3);
										if(sw_in(1) = '1' and sw_in(5) = '0') then
											if(co2(2 downto 0) = "001") then
												led_out(2 downto 0) <= "100";
											else
												led_out(2 downto 0) <= "010";
											end if;
										else
											led_out(2 downto 0) <= "001";
										end if;
								end if;
								---------------------------------
						elsif (time = 7) then
							if(co3(7 downto 6) /= "11") then
										led_out(2 downto 0) <= "001";
										led_out(7 downto 5) <= co3(5 downto 3);			
								else
								led_out(7 downto 5) <= co3(5 downto 3);
									if(sw_in(2) = '1' and sw_in(6) = '0') then
										if(co3(2 downto 0) = "001") then
											led_out(2 downto 0) <= "100";
										else
											led_out(2 downto 0) <= "010";
										end if;
									else
										led_out(2 downto 0) <= "001";
									end if;
							end if;
								--------------------------------
						elsif (time = 10) then
							if(co4(7 downto 6) /= "11") then
										led_out(2 downto 0) <= "001";
										led_out(7 downto 5) <= co4(5 downto 3);			
								else	 
								led_out(7 downto 5) <= co4(5 downto 3);
									if(sw_in(3) = '1' and sw_in(7) = '0') then
										if(co4(2 downto 0) = "001") then
											led_out(2 downto 0) <= "100";
										else
											led_out(2 downto 0) <= "010";
										end if;
									else
										led_out(2 downto 0) <= "001";
									end if;
							end if;
		------------                                      --------------------------------                   -------------------------------				elsif (time = 5) then
						elsif (time = 13) then
						if(co5(7 downto 6) /= "11") then
									led_out(2 downto 0) <= "001";
									led_out(7 downto 5) <= co5(5 downto 3);			
							else	 
							led_out(7 downto 5) <= co5(5 downto 3);
								if(sw_in(4) = '0') then
									led_out(2 downto 0) <= "001";
								elsif (sw_in(0) = '1' and sw_in(4) = '1' and time = 13) then
									led_out(2 downto 0) <= "100";
								elsif (sw_in(0) = '1' and sw_in(4) = '1' and time = 14) then
									led_out(2 downto 0) <= "010";
								elsif (sw_in(0) = '1' and sw_in(4) = '1' and time = 15) then
									led_out(2 downto 0) <= "001";
								elsif (sw_in(4) = '1' and sw_in(0) = '0') then
									if(co5(2 downto 0) = "001") then
										led_out(2 downto 0) <= "100";
									else
										led_out(2 downto 0) <= "010";
									end if;
								end if;	
						end if;			
						elsif(time = 16) then
						if(co6(7 downto 6) /= "11") then
									led_out(2 downto 0) <= "001";
									led_out(7 downto 5) <= co6(5 downto 3);			
							else	 
							led_out(7 downto 5) <= co6(5 downto 3);
								if(sw_in(5) = '0') then
									led_out(2 downto 0) <= "001";
								elsif (sw_in(1) = '1' and sw_in(5) = '1' and time = 16) then
									led_out(2 downto 0) <= "100";
								elsif (sw_in(1) = '1' and sw_in(5) = '1' and time = 17) then
									led_out(2 downto 0) <= "010";
								elsif (sw_in(1) = '1' and sw_in(5) = '1' and time = 18) then
									led_out(2 downto 0) <= "001";
								elsif (sw_in(5) = '1' and sw_in(1) = '0') then
									if(co6(2 downto 0) = "001") then
										led_out(2 downto 0) <= "100";
									else
										led_out(2 downto 0) <= "010";
									end if;
								end if;
						end if;

						elsif (time = 19) then
						if(co7(7 downto 6) /= "11") then
									led_out(2 downto 0) <= "001";
									led_out(7 downto 5) <= co7(5 downto 3);			
							else	 
							led_out(7 downto 5) <= co7(5 downto 3);
								if(sw_in(6) = '0') then
									led_out(2 downto 0) <= "001";
								elsif (sw_in(2) = '1' and sw_in(6) = '1' and time = 19) then
									led_out(2 downto 0) <= "100";
								elsif (sw_in(2) = '1' and sw_in(6) = '1' and time = 20) then
									led_out(2 downto 0) <= "010";
								elsif (sw_in(2) = '1' and sw_in(6) = '1' and time = 21) then
									led_out(2 downto 0) <= "001";
								elsif (sw_in(6) = '1' and sw_in(2) = '0') then
									if(co7(2 downto 0) = "001") then
										led_out(2 downto 0) <= "100";
									else
										led_out(2 downto 0) <= "010";
									end if;
								end if;
						end if;

						elsif (time = 22) then
						if(co8(7 downto 6) /= "11") then
									led_out(2 downto 0) <= "001";
									led_out(7 downto 5) <= co8(5 downto 3);			
							else	 
							led_out(7 downto 5) <= co8(5 downto 3);
								if(sw_in(7) = '0') then
									led_out(2 downto 0) <= "001";
								elsif (time = 22 and sw_in(3) = '1' and sw_in(7) = '1' ) then
									led_out(2 downto 0) <= "100";
								elsif (time = 23 and sw_in(3) = '1' and sw_in(7) = '1' ) then
									led_out(2 downto 0) <= "010";
								elsif (sw_in(3) = '1' and sw_in(7) = '1' and time = 24) then
									led_out(2 downto 0) <= "001";
								elsif (sw_in(7) = '1' and sw_in(3) = '0') then
									if(co8(2 downto 0) = "001") then
										led_out(2 downto 0) <= "100";
									else
										led_out(2 downto 0) <= "010";
									end if;
								end if;
						end if;
						elsif (time = 14) then
						if(co5(7 downto 6) /= "11") then
									led_out(2 downto 0) <= "001";
									led_out(7 downto 5) <= co5(5 downto 3);			
							else	 
							led_out(7 downto 5) <= co5(5 downto 3);
								if(sw_in(4) = '0') then
									led_out(2 downto 0) <= "001";
								elsif (sw_in(0) = '1' and sw_in(4) = '1' and time = 13) then
									led_out(2 downto 0) <= "100";
								elsif (sw_in(0) = '1' and sw_in(4) = '1' and time = 14) then
									led_out(2 downto 0) <= "010";
								elsif (sw_in(0) = '1' and sw_in(4) = '1' and time = 15) then
									led_out(2 downto 0) <= "001";
								elsif (sw_in(4) = '1' and sw_in(0) = '0') then
									if(co5(2 downto 0) = "001") then
										led_out(2 downto 0) <= "100";
									else
										led_out(2 downto 0) <= "010";
									end if;
								end if;	
						end if;			
						elsif(time = 17) then
						if(co6(7 downto 6) /= "11") then
									led_out(2 downto 0) <= "001";
									led_out(7 downto 5) <= co6(5 downto 3);			
							else	 
							led_out(7 downto 5) <= co6(5 downto 3);
								if(sw_in(5) = '0') then
									led_out(2 downto 0) <= "001";
								elsif (sw_in(1) = '1' and sw_in(5) = '1' and time = 16) then
									led_out(2 downto 0) <= "100";
								elsif (sw_in(1) = '1' and sw_in(5) = '1' and time = 17) then
									led_out(2 downto 0) <= "010";
								elsif (sw_in(1) = '1' and sw_in(5) = '1' and time = 18) then
									led_out(2 downto 0) <= "001";
								elsif (sw_in(5) = '1' and sw_in(1) = '0') then
									if(co6(2 downto 0) = "001") then
										led_out(2 downto 0) <= "100";
									else
										led_out(2 downto 0) <= "010";
									end if;
								end if;
						end if;

						elsif (time = 20) then
						if(co7(7 downto 6) /= "11") then
									led_out(2 downto 0) <= "001";
									led_out(7 downto 5) <= co7(5 downto 3);			
							else	 
							led_out(7 downto 5) <= co7(5 downto 3);
								if(sw_in(6) = '0') then
									led_out(2 downto 0) <= "001";
								elsif (sw_in(2) = '1' and sw_in(6) = '1' and time = 19) then
									led_out(2 downto 0) <= "100";
								elsif (sw_in(2) = '1' and sw_in(6) = '1' and time = 20) then
									led_out(2 downto 0) <= "010";
								elsif (sw_in(2) = '1' and sw_in(6) = '1' and time = 21) then
									led_out(2 downto 0) <= "001";
								elsif (sw_in(6) = '1' and sw_in(2) = '0') then
									if(co7(2 downto 0) = "001") then
										led_out(2 downto 0) <= "100";
									else
										led_out(2 downto 0) <= "010";
									end if;
								end if;
						end if;

						elsif (time = 23) then
						if(co8(7 downto 6) /= "11") then
									led_out(2 downto 0) <= "001";
									led_out(7 downto 5) <= co8(5 downto 3);			
							else	 
							led_out(7 downto 5) <= co8(5 downto 3);
								if(sw_in(7) = '0') then
									led_out(2 downto 0) <= "001";
								elsif (time = 22 and sw_in(3) = '1' and sw_in(7) = '1' ) then
									led_out(2 downto 0) <= "100";
								elsif (time = 23 and sw_in(3) = '1' and sw_in(7) = '1' ) then
									led_out(2 downto 0) <= "010";
								elsif (sw_in(3) = '1' and sw_in(7) = '1' and time = 24) then
									led_out(2 downto 0) <= "001";
								elsif (sw_in(7) = '1' and sw_in(3) = '0') then
									if(co8(2 downto 0) = "001") then
										led_out(2 downto 0) <= "100";
									else
										led_out(2 downto 0) <= "010";
									end if;
								end if;
						end if;
						elsif (time = 15) then
						if(co5(7 downto 6) /= "11") then
									led_out(2 downto 0) <= "001";
									led_out(7 downto 5) <= co5(5 downto 3);			
							else	 
							led_out(7 downto 5) <= co5(5 downto 3);
								if(sw_in(4) = '0') then
									led_out(2 downto 0) <= "001";
								elsif (sw_in(0) = '1' and sw_in(4) = '1' and time = 13) then
									led_out(2 downto 0) <= "100";
								elsif (sw_in(0) = '1' and sw_in(4) = '1' and time = 14) then
									led_out(2 downto 0) <= "010";
								elsif (sw_in(0) = '1' and sw_in(4) = '1' and time = 15) then
									led_out(2 downto 0) <= "001";
								elsif (sw_in(4) = '1' and sw_in(0) = '0') then
									if(co5(2 downto 0) = "001") then
										led_out(2 downto 0) <= "100";
									else
										led_out(2 downto 0) <= "010";
									end if;
								end if;
						end if;				
						elsif(time = 18) then
						if(co6(7 downto 6) /= "11") then
									led_out(2 downto 0) <= "001";
									led_out(7 downto 5) <= co6(5 downto 3);			
							else	 
							led_out(7 downto 5) <= co6(5 downto 3);
								if(sw_in(5) = '0') then
									led_out(2 downto 0) <= "001";
								elsif (sw_in(1) = '1' and sw_in(5) = '1' and time = 16) then
									led_out(2 downto 0) <= "100";
								elsif (sw_in(1) = '1' and sw_in(5) = '1' and time = 17) then
									led_out(2 downto 0) <= "010";
								elsif (sw_in(1) = '1' and sw_in(5) = '1' and time = 18) then
									led_out(2 downto 0) <= "001";
								elsif (sw_in(5) = '1' and sw_in(1) = '0') then
									if(co6(2 downto 0) = "001") then
										led_out(2 downto 0) <= "100";
									else
										led_out(2 downto 0) <= "010";
									end if;
								end if;
						end if;
						elsif (time = 21) then
						if(co7(7 downto 6) /= "11") then
									led_out(2 downto 0) <= "001";
									led_out(7 downto 5) <= co7(5 downto 3);			
							else	 
							led_out(7 downto 5) <= co7(5 downto 3);
								if(sw_in(6) = '0') then
									led_out(2 downto 0) <= "001";
								elsif (sw_in(2) = '1' and sw_in(6) = '1' and time = 19) then
									led_out(2 downto 0) <= "100";
								elsif (sw_in(2) = '1' and sw_in(6) = '1' and time = 20) then
									led_out(2 downto 0) <= "010";
								elsif (sw_in(2) = '1' and sw_in(6) = '1' and time = 21) then
									led_out(2 downto 0) <= "001";
								elsif (sw_in(6) = '1' and sw_in(2) = '0') then
									if(co7(2 downto 0) = "001") then
										led_out(2 downto 0) <= "100";
									else
										led_out(2 downto 0) <= "010";
									end if;
								end if;
						end if;
						elsif (time = 24) then
						if(co8(7 downto 6) /= "11") then
									led_out(2 downto 0) <= "001";
									led_out(7 downto 5) <= co8(5 downto 3);			
							else	 
							led_out(7 downto 5) <= co8(5 downto 3);
								if(sw_in(7) = '0') then
									led_out(2 downto 0) <= "001";
		
								elsif (time = 22 and sw_in(3) = '1' and sw_in(7) = '1' ) then
									led_out(2 downto 0) <= "100";
								elsif (time = 23 and sw_in(3) = '1' and sw_in(7) = '1' ) then
									led_out(2 downto 0) <= "010";
								elsif (sw_in(3) = '1' and sw_in(7) = '1' and time = 24) then
									led_out(2 downto 0) <= "001";
								elsif (sw_in(7) = '1' and sw_in(3) = '0') then
									if(co8(2 downto 0) = "001") then
										led_out(2 downto 0) <= "100";
									else
										led_out(2 downto 0) <= "010";
									end if;
								end if;
						end if;

						elsif (time = 25) then 
							led_out <= "00000000" ; 
							mainstate <= "10011" ;
							-- 	state <= '0' ; 
							time := 0 ;
							counter := 0;
							count3 <= "000" ; 
							count2 <= "00" ; 
							led_out <= "00000000"; 
							co1 <= "00000000";
							co2 <= "00000000";
							co3 <= "00000000";
							co4 <= "00000000";
							co5 <= "00000000";
							co6 <= "00000000";
							co7 <= "00000000";
							co8 <= "00000000";
							-- 	enable2 <= '1';	
							creset <= '1';
							ack_part <= "11111111";
					end if;

--------------------------------------------------------------------------
--------------------------------------------------------------------------------
		--- S3 macro state, send data to backend computer.
		elsif( mainstate = "10011" ) then
			led_out <= "10000011"  ;
			if (up_now = '1') then
			stay <= '1' ; 
			elsif stay = '0' then
			iniput(31 downto 16) <= "0000000000000000";
			iniput(15 downto 8) <= ack_part;
			iniput(7 downto 0) <= sw_in ;
			creset <= '0' ;
			ministate <= "001" ;
			led_out <= sw_in ;
			mainstate <= "10100" ;  
			end if ;  
			if( down_now = '1' and stay = '1') then 
			iniput(31 downto 16) <= "0000000000000000";
					iniput(7 downto 0) <= sw_in ;
					iniput(15 downto 8) <= "00000000";
					creset <= '0' ;
					ministate <= "001" ;
					led_out <= sw_in ;  
			end if  ;
			if (ministate = "001") then 
			if chanAddr_in = chan_w and f2hReady_in = '1' then 
	 			if encryption_over = '1' then
				if(count2 = "00") then
				send_data <= ciphertext_out(31 downto 24);
				count2 <= "01";
				elsif(count2 = "01") then
			
				send_data <= ciphertext_out(23 downto 16);
				count2 <= "10";
				elsif(count2 = "10") then
			
				send_data <= ciphertext_out(15 downto 8);
				count2 <= "11";
				elsif(count2 = "11") then
				led_out <= ciphertext_out(7 downto 0) ; 
				send_data <= ciphertext_out(7 downto 0);
				count2 <= "00";
				ministate <= "000" ;
				mainstate <= "10100" ; 
				stay <= '0' ; 
				creset <= '1' ; 
				end if ;
				end if ; 
			end if ; 
			end if;
		
			-- S4 macro state , send data to another using uart.
		elsif(mainstate = "10100")then 
					--led_out <= "00011111"; 
					if(left_now = '1') then 
					stay <= '1' ; 
					elsif stay = '0' then
					mainstate <= "10101" ; 
					end if ; 
					if(count = 0 and right_now = '1')then
						uart_tx_enable <= '1';
						uart_tx_data <= sw_in;
						count <= 1;
					elsif(count = 1)then
						count <= 2;
						uart_tx_enable <= '0';
					elsif(count = 2 and uart_tx_ready = '1' )then
						mainstate <= "10101";
						count <= 0 ;
						stay <= '0' ; 				
					end if;
		--- S5 macro state. 			
		elsif(mainstate = "10101")then -- Check if data came on UART port
				if(datauc = '1')then
						led_out <= uart_data;
						datauc <= '0' ; 
						----------------------------
					if   (uart_data(5 downto 3) = "000") then
						co11 <= uart_data;
					elsif(uart_data(5 downto 3) = "001") then
						co12 <= uart_data;
					elsif(uart_data(5 downto 3) = "010") then
						co13 <= uart_data;
					elsif(uart_data(5 downto 3) = "011") then
						co14 <= uart_data;
					elsif(uart_data(5 downto 3) = "100") then
						co15 <= uart_data;
					elsif(uart_data(5 downto 3) = "101") then
						co16 <= uart_data;
					elsif(uart_data(5 downto 3) = "110") then
						co17 <= uart_data;
					elsif(uart_data(5 downto 3) = "111") then
						co18 <= uart_data;
					end if;

					else
						mainstate <= "10110" ; 
				end if;	

		---------S6 macro state.
		elsif(mainstate = "10110")then 
					counter := counter + 1;
					if(counter = 5) then
						time := time + 1;
					end if;

					if( time = 0 ) then
					--led_out <= "11111111";
					elsif( time = 25) then
					led_out <= "00000000";				
					counter := 0;
					time := 0;
					mainstate <= "00001";
					help <= "000" ; 				
					end if;	 
		end if ;
		end if;
	end process;
	coord <= "00000000000000000000000000100010" ; 
	K <= "11111111111111111111111111111111" ; 
	chan_w <= "0000100";
	chan_r <= "0000101";
	f2hValid_out <= '1';
	h2fReady_out <= '1'; -- Mess with this also                                                   
	with chanAddr_in select f2hData_out <=
		send_data when "0000100",
		"00000000" when others;

	anode_out <= (others => '0');
	sseg_out <= (others => '0');
	
	encrypt : entity work.encrypter 
		port map(
			clock => clk_in,
			K => K,
			C => ciphertext_out,
			P => iniput,
			reset => creset,
			enable => '1',
			done => encryption_over
		);
	decrypt : entity work.decrypter 
		port map(
			clock => clk_in,
			K => K,
				C => host_data1 ,
				P => data_out1 , 
			reset => creset,
			enable => '1',
			done => done_dec1 
		);
	decrypt2: entity work.decrypter
				port map (
				clock => clk_in ,
				K => "11111111111111111111111111111111",
				C => host_data2 ,
				P => data_out2 , 
				enable => '1' ,
				reset => creset,
				done => done_dec2 
				) ; 
	debouncer1 : entity work.debouncer
		port map(
			clk => clk_in,
			button => reset,
			button_deb => center_now
		);
	debouncer2 : entity work.debouncer
		port map(
			clk => clk_in,
			button => upbutton,
			button_deb => up_now
		);
	debouncer3 : entity work.debouncer
		port map(
			clk => clk_in,
			button => downbutton,
			button_deb => down_now
		);
	debouncer4 : entity work.debouncer
		port map(
			clk => clk_in,
			button => rightbutton,
			button_deb => right_now
		);
	debouncer5 : entity work.debouncer
		port map(
			clk => clk_in,
			button => leftbutton,
			button_deb => left_now
		);
end architecture;
