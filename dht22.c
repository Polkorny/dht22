#include <bcm2835.h>
#include <stdio.h>

#define DHTPIN RPI_GPIO_P1_7

int read_dht22_data(float *humidity, float *temperature) {
    uint8_t laststate = HIGH;
    uint8_t counter = 0;
    uint8_t j = 0, i;
    uint8_t bits[40];

    // Enviar um pulso baixo (start)
    bcm2835_gpio_write(DHTPIN, BCM2835_LOW);
    bcm2835_delayMicroseconds(18000);  // 18ms
    bcm2835_gpio_write(DHTPIN, BCM2835_HIGH);
    bcm2835_delayMicroseconds(40);

    // Configurar o pino como entrada
    bcm2835_gpio_fsel(DHTPIN, BCM2835_GPIO_FSEL_INPT);

    // Esperar por uma resposta
    for (i=0; i<40; i++) {
        counter = 0;
        while (bcm2835_gpio_lev(DHTPIN) == laststate) {
            counter++;
            bcm2835_delayMicroseconds(1);
            if (counter == 255) break;
        }
        laststate = bcm2835_gpio_lev(DHTPIN);
        if (counter > 50) bits[j] = 1;
        else if (counter > 10) bits[j] = 0;
        j++;
    }

    // Verificar se todos os 40 bits foram recebidos corretamente
    if ((bits[0] == 0) && (bits[1] == 1)) {
        // Calcular a temperatura e umidade
        // ... (implementação da lógica de cálculo)
        *humidity = calculated_humidity;
        *temperature = calculated_temperature;
        return 0;
    } else {
        return -1;
    }
}

int main(void) {
    float h, t;

    if (!bcm2835_init())
        return 1;

    if (read_dht22_data(&h, &t) == 0) {
        printf("Umidade: %.2f%%  Temperatura: %.2f *C\n", h, t);
    } else {
        printf("Falha ao ler dados!\n");
    }

    bcm2835_close();
    return 0;
}

