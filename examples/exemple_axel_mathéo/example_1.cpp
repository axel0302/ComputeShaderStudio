void main() {
    uint x = gl_GlobalInvocationID.x;
    uint y = gl_GlobalInvocationID.y;
    uint p = x + y * WSX; // Position dans le buffer

    // Centre de l'image
    float centerX = WSX * 0.5;
    float centerY = WSY * 0.5;

    // Parametres du cercle et du trait
    float radius = min(WSX, WSY) * 0.4; // Rayon du cercle
    float thickness = 1.5;              // Epaisseur du cercle
    float stop_percent = 0.9;           // Arret du cercle a 90%
    float line_percent = 0.1;           // Longueur du trait vers le centre (entre 0 et 1)

    // Fond blanc par defaut
    data_0[p] = 0xFFFFFFFF; // Blanc

    // Calcul de la distance au centre
    float dx = float(x) - centerX;
    float dy = float(y) - centerY;
    float distance = sqrt(dx * dx + dy * dy);

    // Calcul de l'angle (entre 0 et 2*PI)
    float angle = atan(dy, dx);
    if (angle < 0.0) angle += 6.28318530718; // Normaliser entre 0 et 2*PI

    // Animation : progression du cercle (augmente avec step)
    float animation_progress = mod(float(step) * 0.02, 6.28318530718);

    // Angle d'arret a 90%
    float stop_angle = stop_percent * 6.28318530718;

    // Dessiner le cercle jusqu'a 90% du tour
    if (abs(distance - radius) <= thickness && angle <= animation_progress && angle <= stop_angle) {
        data_0[p] = 0xFF000000; // Noir (trace le cercle)
    }

    // Dessiner le trait vers le centre a partir de 90%
    if (animation_progress >= stop_angle) {
        // Point de depart du trait
        float stop_x = centerX + radius * cos(stop_angle);
        float stop_y = centerY + radius * sin(stop_angle);

        // Point d'arret du trait (interpolation vers le centre)
        float target_x = centerX + (stop_x - centerX) * (1.0 - line_percent);
        float target_y = centerY + (stop_y - centerY) * (1.0 - line_percent);

        // Verifier si (x, y) est sur la ligne
        float t = ((float(x) - stop_x) * (target_x - stop_x) +
                   (float(y) - stop_y) * (target_y - stop_y)) /
                  ((target_x - stop_x) * (target_x - stop_x) +
                   (target_y - stop_y) * (target_y - stop_y));

        float line_x = stop_x + t * (target_x - stop_x);
        float line_y = stop_y + t * (target_y - stop_y);

        float dist_to_line = sqrt((float(x) - line_x) * (float(x) - line_x) +
                                  (float(y) - line_y) * (float(y) - line_y));

        // Si le point est sur le trait vers le centre
        if (dist_to_line <= thickness && t >= 0.0 && t <= 1.0) {
            data_0[p] = 0xFF000000; // Noir (trace le trait)
        }
    }
}
