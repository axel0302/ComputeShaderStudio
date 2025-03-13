// TRACE QUE 1 TRAIT PUIS CERCLE BONNE HAUTEUR MAIS PAS BON ENDROIT
void main() {
    uint x = gl_GlobalInvocationID.x;
    uint y = gl_GlobalInvocationID.y;
    uint p = x + y * WSX; // Position dans le buffer

    // Centre initial de l'image
    float centerX = WSX * 0.5;
    float centerY = WSY * 0.5;

    // Parametres du cercle
    float initial_radius = min(WSX, WSY) * 0.4; // Rayon de depart
    float thickness = 1.5;                      // Epaisseur du cercle
    float stop_percent = 0.9;                   // Arret du cercle a 90%
    float line_length = 0.15;                   // Longueur du trait vers l'interieur
    float shrink_factor = 0.85;                 // Reduction du rayon a chaque boucle

    // Fond blanc par defaut
    data_0[p] = 0xFFFFFFFF; // Blanc

    // Animation : progression du cercle (augmente avec step)
    float animation_progress = float(step) * 0.02;

    // Initialisation des variables
    float radius = initial_radius;
    float accumulated_angle = 0.0;
    float currentCenterX = centerX;
    float currentCenterY = centerY;

    // Calcul de la position actuelle
    float dx = float(x) - currentCenterX;
    float dy = float(y) - currentCenterY;
    float distance = sqrt(dx * dx + dy * dy);
    float angle = atan(dy, dx);
    if (angle < 0.0) angle += 6.28318530718; // Normaliser entre 0 et 2*PI

    // Dessiner les cercles et les traits successifs
    while (radius > 10.0) { // Arreter quand le cercle devient tres petit
        float stop_angle = stop_percent * 6.28318530718;

        // Dessiner le cercle actuel
        if (abs(distance - radius) <= thickness && angle <= animation_progress - accumulated_angle && angle <= stop_angle) {
            data_0[p] = 0xFF000000; // Noir (cercle)
        }

        // Dessiner le trait perpendiculaire (vers l'interieur)
        if (animation_progress - accumulated_angle >= stop_angle) {
            float line_progress = min(1.0, (animation_progress - accumulated_angle - stop_angle) / 0.3);

            // Parcourir toute la longueur du trait
            for (float t = 0.0; t <= line_progress; t += 0.01) {
                float inner_radius = radius - radius * line_length * t;
                float inner_x = currentCenterX + inner_radius * cos(stop_angle);
                float inner_y = currentCenterY + inner_radius * sin(stop_angle);

                float inner_distance = sqrt((float(x) - inner_x) * (float(x) - inner_x) + (float(y) - inner_y) * (float(y) - inner_y));

                if (inner_distance <= thickness) {
                    data_0[p] = 0xFF000000; // Noir (trait)
                }
            }
        }

        // Calcul de la fin du trait pour le prochain cercle
        float end_x = currentCenterX + (radius - radius * line_length) * cos(stop_angle);
        float end_y = currentCenterY + (radius - radius * line_length) * sin(stop_angle);

        // Mise a jour du centre et du rayon pour le prochain cercle
        currentCenterX = end_x;
        currentCenterY = end_y;
        radius *= shrink_factor;

        // Mise a jour de l'angle accumule
        accumulated_angle += stop_angle + 0.3;
    }
}
