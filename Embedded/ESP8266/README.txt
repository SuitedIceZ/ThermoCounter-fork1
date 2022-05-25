NodeMCU - Master I2C Receive data from STM32 slave
	D1 is for I2C SDA
	D2 is for I2C SCL

**Currently Prototype for this stm32 
  /* USER CODE BEGIN WHILE */
  char buffer[100];
  char mode = '0';
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, mode-'0');
	  HAL_I2C_Slave_Transmit(&hi2c1, &mode, sizeof(mode), 100);
	  mode = (mode == '0' ? '1' : '0');
	  HAL_Delay(1000);
  }
  /* USER CODE END 3 */