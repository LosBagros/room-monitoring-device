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

- Umístěte zařízení blízko Wi-Fi routeru, aby bylo zajištěno silné a stabilní připojení. Nejlépe ve stejné místnosti jako je router.
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

## Známé problémy

- Měření teploty a vlhkosti z PCB je ovlivněno zahříváním desky, což vede k nepřesným hodnotám.
- Světelný senzor nefunguje v této verzi PCB kvůli chybě v návrhu schématu.
- Wi-Fi anténa je slabá, což může vést k nestabilnímu připojení, pokud je zařízení daleko od routeru.

## Online Dashboard

[**IOT.BAGROS.EU**](https://iot.bagros.eu/d/be7hw0wxuy1vkc/co2?orgId=1&from=now-3h&to=now&timezone=browser&kiosk)

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

## Varování

<!-- prettier-ignore-start -->
<!-- markdownlint-disable -->
<table>
    <tr>
        <td align="center">
            <img src="https://raw.githubusercontent.com/LosBagros/room-monitoring-device/refs/heads/main/img/icons/microwave-off.svg" width="100px;" alt="Mikrovlnná trouba ikona" />
            <br />
            <sub><b>Nedávejte zařízení do mikrovlnné trouby</b></sub>
        </td>
        <td align="center">
            <img src="https://raw.githubusercontent.com/LosBagros/room-monitoring-device/refs/heads/main/img/icons/bucket-droplet.svg" width="100px;" alt="Voda ikona" />
            <br />
            <sub><b>Nevystavujte vodě</b></sub>
        </td>
        <td align="center">
            <img src="https://raw.githubusercontent.com/LosBagros/room-monitoring-device/refs/heads/main/img/icons/flame.svg" width="100px;" alt="Hořlavé materiály ikona" />
            <br />
            <sub><b>Nepokládejte blízko hořlavých materiálů</b></sub>
        </td>
    </tr>
    <tr>
        <td align="center">
            <img src="https://raw.githubusercontent.com/LosBagros/room-monitoring-device/refs/heads/main/img/icons/gavel.svg" width="100px;" alt="Kladivo ikona" />
            <br />
            <sub><b>Netestujte odolnost kladivem</b></sub>
        </td>
        <td align="center">
            <img src="https://raw.githubusercontent.com/LosBagros/room-monitoring-device/refs/heads/main/img/icons/bolt.svg" width="100px;" alt="Napájení ikona" />
            <br />
            <sub><b>Nepoužívejte jiný zdroj napájení než 5V</b></sub>
        </td>
        <td align="center">
            <img src="https://raw.githubusercontent.com/LosBagros/room-monitoring-device/refs/heads/main/img/icons/paw-off.svg" width="100px;" alt="Zvířata ikona" />
            <br />
            <sub><b>Nepovolujte domácím mazlíčkům monitorovat vzduch</b></sub>
        </td>
    </tr>
</table>
<!-- markdownlint-restore -->
<!-- prettier-ignore-end -->
