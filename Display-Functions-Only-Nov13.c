Initialize LED Display
   /* Reset LED Display (RES# = PB4):
       - make pin PB4 = 0, wait for a few ms
       - make pin PB4 = 1, wait for a few ms
    */
    …
   GPIOB->ODR &= ~GPIO_ODR_4; // Set PB4 (RES#) to 0 (reset) 
   delay_ms(5); // Wait a few milliseconds 
   GPIOB->ODR |= GPIO_ODR_4; // Set PB4 (RES#) to 1 (end reset) 
    delay_ms(5); // Wait a few milliseconds after reset


//
// Send initialization commands to LED Display
//
    for ( unsigned int i = 0; i < sizeof( oled_init_cmds ); i++ )
    {
        oled_Write_Cmd( oled_init_cmds[i] );
    }


    /* Fill LED Display data memory (GDDRAM) with zeros: 
       - for each PAGE = 0, 1, ..., 7
           set starting SEG = 0
           call oled_Write_Data( 0x00 ) 128 times
    */
// Fill OLED display data memory (GDDRAM) with zeros 
for (unsigned int page = 0; page < 8; page++) { // For each PAGE 0 to 7 
// Set the current page ??
// Start from column 0 ??
for (unsigned int seg = 0; seg < 128; seg++) { // Write 128 columns per page          oled_Write_Data(0x00); // Send 0x00 to clear each column segment 
} 
}

    …

void oled_Write_Cmd( unsigned char cmd )
{
    ... // make PB6 = CS# = 1
           GPIOB->ODR |= GPIO_ODR_6;
    ... // make PB7 = D/C# = 0
          GPIOB->ODR &= ~GPIO_ODR_7;
    ... // make PB6 = CS# = 0
        GPIOB->ODR &= ~GPIO_ODR_6;
        oled_Write( cmd );
    ... // make PB6 = CS# = 1
       GPIOB->ODR |= GPIO_ODR_6;
}

void oled_Write_Data( unsigned char data )
{
    ... // make PB6 = CS# = 1
       GPIOB->ODR |= GPIO_ODR_6;
    ... // make PB7 = D/C# = 1
       GPIOB->ODR |= GPIO_ODR_7;
    ... // make PB6 = CS# = 0 
       GPIOB->ODR &= ~GPIO_ODR_6;
    oled_Write( data );
    ... // make PB6 = CS# = 1
      GPIOB->ODR |= GPIO_ODR_6;
}



LED Display Interfacing

void oled_Write( unsigned char Value )
{

    /* Wait until SPI1 is ready for writing (TXE = 1 in SPI1_SR) */

    while (!(SPI1->SR & SPI_SR_TXE)) 
{ // Wait for TXE bit to set, which indicates the transmit buffer is empty }

    /* Send one 8-bit character:
       - This function also sets BIDIOE = 1 in SPI1_CR1
    */
    HAL_SPI_Transmit( &SPI_Handle, &Value, 1, HAL_MAX_DELAY );


    /* Wait until transmission is complete (TXE = 1 in SPI1_SR) */
while (!(SPI1->SR & SPI_SR_TXE)) 
{ // Wait for TXE bit to set, which indicates the transmit buffer is empty }

    ...

}
void refresh_OLED( void )
{
    // Buffer size = at most 16 characters per PAGE + terminating '\0'
    unsigned char Buffer[17]; 

    snprintf( Buffer, sizeof( Buffer ), "R: %5u Ohms", Res );
    /* Buffer now contains your character ASCII codes for LED Display
       - select PAGE (LED Display line) and set starting SEG (column)
       - for each c = ASCII code = Buffer[0], Buffer[1], ...,
           send 8 bytes in Characters[c][0-7] to LED Display
    */

    // Set page and column to display "F: ... Hz" at the next page
    oled_Write_Cmd(0xB2); // command to set page
    oled_Write_Cmd(0x05); // Set starting column lower nibble
    oled_Write_Cmd(0x10); // Set starting column higher nibble

   // Loop through each character in Buffer 
for (int i = 0; Buffer[i] != '\0'; i++) 
{ 
unsigned char c = Buffer[i]; 
for (int j = 0; j < 8; j++) { 
oled_Write_Data(Characters[c][j]); // Send character data for display 
} 
}


    snprintf( Buffer, sizeof( Buffer ), "F: %5u Hz", Freq );
    /* Buffer now contains your character ASCII codes for LED Display
       - select PAGE (LED Display line) and set starting SEG (column)
       - for each c = ASCII code = Buffer[0], Buffer[1], ...,
           send 8 bytes in Characters[c][0-7] to LED Display
    */
    oled_Write_Cmd(0xB?); // command to set page
    oled_Write_Cmd(0x0?); // Set starting column lower nibble
    oled_Write_Cmd(0x1?); // Set starting column higher nibble

  // Loop through each character in Buffer 
for (int i = 0; Buffer[i] != '\0'; i++) 
{ 
unsigned char c = Buffer[i]; 
for (int j = 0; j < 8; j++) { 
oled_Write_Data(Characters[c][j]); // Send character data for display 
} 
}


	/* Wait for ~100 ms (for example) to get ~10 frames/sec refresh rate 
       - You should use TIM3 to implement this delay (e.g., via polling)
    */
    //set TIM3 to 48MHz
    TIM3->CNT= systemCoreClock;
    //start timer
    TIM3->CR1 |= TIM_CR1_CEN;
    // wain until timer reaches zero
    while(TIM3->CNT != 0);
    //stop timer
    TIM3->CR1 |= TIM_CR1_UDIS;
/*
TIM3->CNT = 0; // Reset TIM3 counter 
TIM3->CR1 |= TIM_CR1_CEN; // Enable TIM3 
while (TIM3->CNT < 1000) { 
// Polling TIM3 count until it reaches ~100 ms 
} 
TIM3->CR1 &= ~TIM_CR1_CEN; // Disable TIM3 after delay
*/

}

