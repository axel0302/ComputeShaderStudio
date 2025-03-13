// Cercle anime (apparition et disparition)
void main() {
    uint x = gl_GlobalInvocationID.x;
    uint y = gl_GlobalInvocationID.y;
    uint p = x + y * WSX; // Position dans le buffer

    // Centre du cercle (milieu de l'image)
    float centerX = WSX / 2.0;
    float centerY = WSY / 2.0;

    // Rayon du cercle et epaisseur
    float radius = min(WSX, WSY) / 3.0; // Ajuste le rayon ici
    float thickness = 1;              // Ajuste l'epaisseur ici

    // Calcul de la distance au centre
    float dx = float(x) - centerX;
    float dy = float(y) - centerY;
    float distance = sqrt(dx * dx + dy * dy);

    // Calcul de l'angle en radians (0 a 2*PI)
    float angle = atan(dy, dx);
    if (angle < 0.0) angle += 2.0 * 3.14159265359;

    // Fond blanc par defaut
    data_0[p] = 0xFFFFFFFF; // Blanc

    // Animation : cercle qui se dessine et s'efface
    float animation_progress = float(step % 360) * (3.14159265359 / 180.0); // de 0 a 2*PI

    // Si le point est dans l'epaisseur du cercle et dans l'angle anime
    if (abs(distance - radius) <= thickness && angle <= animation_progress) {
        data_0[p] = 0xFF000000; // Noir
    }
}