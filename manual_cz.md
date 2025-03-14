# Manuál zařízení pro monitorování místnosti

Odkaz na [Anglickou verzi](manual.md)

Postup pro instalaci a používání zařízení **Room Monitoring Device**, které měří **koncentraci CO₂**, **teplotu** a **vlhkost vzduchu** v místnosti.

## ⚠️ Důležité bezpečnostní upozornění

> **Nenechávejte zařízení bez dozoru.**
>
> Ačkoli bylo zařízení v praxi stabilní a spolehlivé, **jedná se o vlastní elektronické zařízení a nelze zcela vyloučit poruchu nebo riziko požáru**.
>
> Vždy umisťujte zařízení na **nehořlavý povrch**, držte jej **mimo dosah hořlavých materiálů** a **pravidelně kontrolujte jeho provoz**.
>
> **Používejte pouze 5V napájecí adaptéry.**

## Pokyny pro instalaci

### 1. Napájení

- Použijte stabilní 5V USB-C napájecí zdroj (např. nabíječka pro mobilní telefon).
- Zajistěte nepřerušované napájení pro spolehlivý provoz a přenos dat.

### 2. Připojení k Wi-Fi

- Umístěte zařízení blízko Wi-Fi routeru, aby bylo zajištěno silné a stabilní připojení.
- Ujistěte se, že vaše Wi-Fi síť funguje na frekvenci 2.4 GHz. Zařízení nepodporuje 5 GHz sítě.
- Po zapnutí zařízení vytvoří vlastní Wi-Fi síť s názvem `RoomMonitorAP` a heslem `12345678`.
- Připojte se k této síti a otevřete prohlížeč na adrese `http://192.168.4.1`.
- Zadejte název vaší Wi-Fi sítě (SSID) a heslo pro připojení zařízení do vaší domácí sítě.

### 3. Doporučené umístění

- Umístěte zařízení mimo:
  - Přímého slunečního světla
  - Topných nebo chladicích jednotek
  - Oken a dveří
- Zajistěte volný proud vzduchu kolem zařízení — neumisťujte jej do uzavřených prostor.

## Online Dashboard

[**Odkaz na Dashboard**](https://iot.bagros.eu/d/be7hw0wxuy1vkc/co2?orgId=1&from=now-3h&to=now&timezone=browser&kiosk)

Dashboard zobrazuje:

- **Koncentraci CO₂ (ppm)**
- **Teplotu místnosti (°C)**
- **Relativní vlhkost vzduchu (%)**

## LED Indikace

| Barva LED                | Hladina CO₂ (ppm) | Akce                                     |
| ------------------------ | ----------------- | ---------------------------------------- |
| 🟢 Dobrá                 | `< 1000 ppm`      | Není potřeba žádná akce                  |
| 🟡 Střední               | `1000 - 2000 ppm` | Zvýšit větrání                           |
| 🔴 Špatná                | `> 2000 ppm`      | Okamžitě zlepšit větrání                 |
| 🔵 Senzor nepřipraven    | `N/A`             | Čekání na zahřátí CO₂ senzoru            |
| ⚪ Není připojeno k WiFi | `N/A`             | WiFi není nakonfigurováno nebo připojeno |
