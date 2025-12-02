# Limpa o executável
clean:
	rm -f build/*.out

# Compila o jogo
game:
	mkdir -p build
	gcc src/main.c \
		src/utils.c \
		src/renderer.c \
		src/cartas.c \
		src/combate.c \
		src/inimigo.c \
		src/pilha.c \
		src/turno_inimigos.c \
		src/turno_jogador.c \
		-Iinclude \
		-o build/game.out \
		-lm -g \
		$(shell pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_image-5 allegro_primitives-5 --libs --cflags)
