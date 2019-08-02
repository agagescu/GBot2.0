/***************************************************************
 *
 * External DAC for Alligator Board
 *
 ****************************************************************/

#include "../../inc/MarlinConfig.h"

#if MB(ALLIGATOR)

#include "dac_dac084s085.h"

#include "../../Marlin.h"
#include "../../module/stepper.h"
#include "../../HAL/shared/Delay.h"

dac084s085::dac084s085() { }

void dac084s085::begin() {
  uint8_t externalDac_buf[] = { 0x20, 0x00 }; // all off

  // All SPI chip-select HIGH
  SET_OUTPUT(DAC0_SYNC);
  #if EXTRUDERS > 1
    SET_OUTPUT(DAC1_SYNC);
  #endif
  cshigh();
  spiBegin();

  //init onboard DAC
  DELAY_US(2);
  WRITE(DAC0_SYNC, LOW);
  DELAY_US(2);
  WRITE(DAC0_SYNC, HIGH);
  DELAY_US(2);
  WRITE(DAC0_SYNC, LOW);

  spiSend(SPI_CHAN_DAC, externalDac_buf, COUNT(externalDac_buf));
  WRITE(DAC0_SYNC, HIGH);

  #if EXTRUDERS > 1
    //init Piggy DAC
    DELAY_US(2);
    WRITE(DAC1_SYNC, LOW);
    DELAY_US(2);
    WRITE(DAC1_SYNC, HIGH);
    DELAY_US(2);
    WRITE(DAC1_SYNC, LOW);

    spiSend(SPI_CHAN_DAC, externalDac_buf, COUNT(externalDac_buf));
    WRITE(DAC1_SYNC, HIGH);
  #endif

  return;
}

void dac084s085::setValue(const uint8_t channel, const uint8_t value) {
  if (channel >= 7) return; // max channel (X,Y,Z,E0,E1,E2,E3)

  const uint8_t externalDac_buf[] = {
    0x10 | ((channel > 3 ? 7 : 3) - channel << 6) | (value >> 4),
    0x00 | (value << 4)
  };

  // All SPI chip-select HIGH
  cshigh();

  if (channel > 3) {        // DAC Piggy E1,E2,E3
    WRITE(DAC1_SYNC, LOW);
    DELAY_US(2);
    WRITE(DAC1_SYNC, HIGH);
    DELAY_US(2);
    WRITE(DAC1_SYNC, LOW);
  }
  else {                    // DAC onboard X,Y,Z,E0
    WRITE(DAC0_SYNC, LOW);
    DELAY_US(2);
    WRITE(DAC0_SYNC, HIGH);
    DELAY_US(2);
    WRITE(DAC0_SYNC, LOW);
  }

  DELAY_US(2);
  spiSend(SPI_CHAN_DAC, externalDac_buf, COUNT(externalDac_buf));
}

void dac084s085::cshigh() {
  WRITE(DAC0_SYNC, HIGH);
  #if EXTRUDERS > 1
    WRITE(DAC1_SYNC, HIGH);
  #endif
  WRITE(SPI_EEPROM1_CS, HIGH);
  WRITE(SPI_EEPROM2_CS, HIGH);
  WRITE(SPI_FLASH_CS, HIGH);
  WRITE(SS_PIN, HIGH);
}

#endif // MB(ALLIGATOR)
