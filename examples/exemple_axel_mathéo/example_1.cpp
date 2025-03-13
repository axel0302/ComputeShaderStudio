void main() {
    uint x = gl_GlobalInvocationID.x;
    uint y = gl_GlobalInvocationID.y;
    uint p = x + y * WSX;

    // Definition des couleurs
    int WHITE = 0xFFFFFFFF; // Fond blanc
    int BLACK = 0xFF000000; // Contour noir

    // Definir le centre du cercle et son rayon
    int cx = int(WSX) / 2;
    int cy = int(WSY) / 2;
    
    // Calculer la distance au centre
    int dx = int(x) - cx;
    int dy = int(y) - cy;
    int distance_squared = dx * dx + dy * dy;
    
    // Calculer l'angle du pixel par rapport au centre
    float angle = atan(float(dy), float(dx));
    if (angle < 0.0) angle += 3.14159 * 2.0;
    
    // Convertir l'angle en valeur normalisée (0-1)
    float angle_normalized = angle / (3.14159 * 2.0);
    
    // Paramètres du labyrinthe
    int num_circles = 5;                // Nombre de cercles concentriques
    int max_radius = 200;               // Rayon du cercle le plus grand
    int min_radius = 40;                // Rayon du cercle le plus petit
    int thickness = 2;                  // Épaisseur des cercles
    int frames_per_circle = 360;        // Frames pour dessiner un cercle complet
    float completion_percent = 0.95;    // Pourcentage de complétion de chaque cercle (0.95 = 95%)
    
    // Calculer le rayon de chaque cercle
    float radius_step = float(max_radius - min_radius) / float(num_circles - 1);
    
    // Initialiser la couleur du pixel à blanc (fond)
    data_0[p] = WHITE;
    
    // Animation basée sur step
    int current_step = step % (frames_per_circle * num_circles);
    int current_circle = current_step / frames_per_circle;
    int circle_progress = current_step % frames_per_circle;
    
    // Pour chaque cercle déjà commencé
    for (int i = 0; i <= current_circle && i < num_circles; i++) {
        // Calculer le rayon du cercle actuel
        float current_radius = float(max_radius) - float(i) * radius_step;
        
        // Calculer les limites du cercle
        float inner_radius_sq = (current_radius - thickness) * (current_radius - thickness);
        float outer_radius_sq = (current_radius + thickness) * (current_radius + thickness);
        
        // Vérifier si le pixel est sur ce cercle
        if (distance_squared >= inner_radius_sq && distance_squared <= outer_radius_sq) {
            // Calculer la progression de l'animation pour ce cercle
            float max_angle = 1.0;
            
            // Si c'est le cercle en cours d'animation
            if (i == current_circle) {
                max_angle = float(circle_progress) / float(frames_per_circle) * completion_percent;
            } else {
                // Cercles déjà complétés
                max_angle = completion_percent;
            }
            
            // Alterner le sens de rotation pour chaque cercle
            float adjusted_angle;
            if (i % 2 == 0) {
                // Sens horaire
                adjusted_angle = angle_normalized;
            } else {
                // Sens anti-horaire
                adjusted_angle = 1.0 - angle_normalized;
            }
            
            // Dessiner le cercle si l'angle est dans la plage animée
            if (adjusted_angle <= max_angle) {
                data_0[p] = BLACK;
            }
        }
    }
}
