# Limpa o executável
clean:
	rm -f *.out

# Compila  o jogo
game:
	gcc main.c \
		utils.c \
		renderer.c \
		cartas.c \
		combate.c \
		inimigo.c \
		pilha.c \
		turno_inimigos.c \
		turno_jogador.c \
		-o game.out \
		-lm -g \
		$(shell pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_image-5 allegro_primitives-5 --libs --cflags)
