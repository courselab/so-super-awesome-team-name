# Exercício 1 | Hello World

Arthur Vergaças Daher Martins | 12542672
01/06/2025

---

Para esta atividade, foi implementada a funcionalidade de visualização de data e horário utilizando o serviço RTC (Real-Time Clock), disponibilizado pela BIOS por meio da interrupção `1Ah`.

A interrupção `1Ah` oferece diversas funcionalidades, selecionadas por meio do registrador `%ah`. As utilizadas nesta implementação foram:

- `02h`: Lê o horário (horas e minutos) do relógio CMOS
- `04h`: Lê a data (dia, mês e ano) do relógio CMOS

Os valores retornados pelo serviço `1Ah` são codificados em BCD (Binary-Coded Decimal). Ou seja, se o horário for 14:20, o serviço `02h` armazenará o valor `1420h` em hexadecimal. O mesmo vale para a data: se for 01/06, será retornado `0106h`, e para o ano 2025, o valor será `2025h`.

Para exibir a data e o horário na tela, é necessário primeiro convertê-los para string. Para isso, foi implementada a função `bcd_to_str`.

Como o bootloader dispõe de apenas 512 bytes para a execução do programa, a implementação foi otimizada para ocupar o menor espaço possível, visando gerar o menor binário. Por exemplo, a função `get_year` foi implementada de forma a reutilizar a chamada ao serviço de data feita pela função `get_date`. Dessa forma, economizam-se duas instruções, com o custo de depender da chamada prévia da função e de que o estado dos registradores não seja alterado entre as chamadas.
