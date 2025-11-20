# 🏰 Slay the Spire – Trabalho Prático (UFMG)

Este projeto é uma implementação, em C com **Allegro 5**, de um jogo inspirado em *Slay the Spire*, desenvolvido como Trabalho Prático da disciplina de Programação da UFMG.

O jogo segue a estrutura de um deck-builder por turnos: o jogador utiliza cartas de ataque, defesa e efeitos especiais para derrotar inimigos enquanto escala uma torre misteriosa. O objetivo é sobreviver a **10 combates** consecutivos — perder um único combate termina a run.

---

## 🎮 Como Jogar

### Controles

| Tecla | Função |
|-------|--------|
| **← / →** | Navegar entre cartas e inimigos |
| **Enter** | Selecionar carta ou confirmar alvo |
| **ESC** | Encerrar o turno do jogador |
| **Q** | Sair do jogo |
| **Espaço** | Reduz a vida de todos os inimigos para 0 (debug) |
| **X** | Reduz a vida do jogador para 1 (debug) |

---

## 🧙 Mecânicas Principais

### Objetivo
- Inicia com **100 pontos de vida**
- Enfrenta **10 combates sequenciais**
- Se o HP do jogador chegar a zero → **Game Over**
- Se derrotar todos os inimigos nos 10 combates → **Vitória**

### Cartas

O baralho inicial possui 20 cartas:

- **10 de ataque**
- **8 de defesa**
- **2 especiais**

Cada carta possui:
- **Tipo:** Ataque, Defesa ou Especial  
- **Custo:** 0 a 3  
- **Efeito:** dano, escudo ou ação especial  

**Cartas especiais** descartam a mão inteira e compram 5 novas cartas.

### Inimigos

Cada combate gera **2 inimigos**, sendo que existe **5% de chance** de um deles ser forte (e no máximo um forte por combate).

Cada inimigo possui:
- Vida aleatória  
- IA cíclica (ataque/defesa)  
- Uma próxima ação sempre visível ao jogador  

---

## ⚔️ Combate

O combate ocorre por turnos:

1. **Turno do jogador**
   - Energia volta para **3**
   - Escudo é removido
   - Compra 5 cartas
   - Pode jogar qualquer carta cujo custo caiba na energia disponível
   - Encerrar turno descarta toda a mão

2. **Turno dos inimigos**
   - Escudo dos inimigos é removido
   - Eles executam suas ações na ordem da IA

O combate termina quando:
- Todos os inimigos chegam a 0 HP (**vitória**)  
- O jogador chega a 0 HP (**derrota**)  

---

## 🖼️ Interface Gráfica

Durante o combate, são exibidos:

- Figura do jogador  
- Figuras dos inimigos (fracos/fortes)  
- Barras de vida proporcionais com HP atual / máximo  
- Quantidade de escudo  
- Próxima ação de cada inimigo  
- Energia disponível  
- Pilha de compra e pilha de descarte  
- Mão do jogador com suas cartas  
- Destaques visuais para seleção de cartas e inimigos  

