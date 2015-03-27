#include <assert.h>
#include <math.h>
#include <time.h>
#include "lib_3d.h"
#include "lib_objet3d.h"
#include "lib_mat.h"
#include "lib_surface.h"


t_maillon* __cree_maillon(t_triangle3d *face, Uint32 couleur)
{
	t_maillon* pt_maillon = NULL;
	pt_maillon = (t_maillon*) malloc(sizeof(t_maillon));
	pt_maillon->face = face;
	pt_maillon->couleur = couleur;
	pt_maillon->pt_suiv = NULL;

	return pt_maillon;
}

void __insere_tete(t_objet3d *pt_objet, t_maillon *pt_maillon)
{
	if (pt_objet->tete == NULL) {
        pt_objet->tete = pt_maillon;
        pt_objet->est_trie = true;
	} else {
        pt_objet->est_trie = pt_objet->est_trie && zmoyen(pt_maillon->face)>=zmoyen((pt_objet->tete)->face);
        pt_maillon->pt_suiv = pt_objet->tete;
        pt_objet->tete = pt_maillon;
	}
}

t_objet3d *objet_vide()
{
	t_objet3d *pt_objet = NULL;

	pt_objet = (t_objet3d *) malloc(sizeof(t_objet3d));
	pt_objet->est_trie = true;
	pt_objet->tete = NULL;

	return pt_objet;
}

t_objet3d *camera()
{
	return objet_vide();
}

t_objet3d* parallelepipede(double lx, double ly, double lz)
{
	t_objet3d *pt_objet = NULL;
	t_point3d *tab_points[8];
	t_triangle3d *t = NULL;

	pt_objet = objet_vide();

	tab_points[0] = definirPoint3d(0,0,0);
	tab_points[1] = definirPoint3d(lx,0,0);
	tab_points[2] = definirPoint3d(lx,ly,0);
	tab_points[3] = definirPoint3d(0,ly,0);
	tab_points[4] = definirPoint3d(0,ly,lz);
	tab_points[5] = definirPoint3d(lx,ly,lz);
	tab_points[6] = definirPoint3d(lx,0,lz);
	tab_points[7] = definirPoint3d(0,0,lz);

    //face de devant
    t = definirTriangle3d(tab_points[0],tab_points[1],tab_points[2]);
    __insere_tete(pt_objet,__cree_maillon(t,echelle_de_couleur(30)));
    t = definirTriangle3d(tab_points[0],tab_points[2],tab_points[3]);
    __insere_tete(pt_objet,__cree_maillon(t,echelle_de_couleur(30)));
    //triangles ayant 2pts commun avec la face de devant
    t= definirTriangle3d(tab_points[0],tab_points[1],tab_points[6]);
    __insere_tete(pt_objet,__cree_maillon(t,echelle_de_couleur(210)));
    t= definirTriangle3d(tab_points[0],tab_points[3],tab_points[4]);
    __insere_tete(pt_objet,__cree_maillon(t,echelle_de_couleur(210)));
    t= definirTriangle3d(tab_points[5],tab_points[1],tab_points[2]);
    __insere_tete(pt_objet,__cree_maillon(t,echelle_de_couleur(210)));
    t= definirTriangle3d(tab_points[5],tab_points[2],tab_points[3]);
    __insere_tete(pt_objet,__cree_maillon(t,echelle_de_couleur(210)));
    //triangles ayant 2pts commun avec la face de derri�re
    t= definirTriangle3d(tab_points[0],tab_points[6],tab_points[7]);
    __insere_tete(pt_objet,__cree_maillon(t,echelle_de_couleur(210)));
    t= definirTriangle3d(tab_points[0],tab_points[4],tab_points[7]);
    __insere_tete(pt_objet,__cree_maillon(t,echelle_de_couleur(210)));
    t= definirTriangle3d(tab_points[5],tab_points[1],tab_points[6]);
    __insere_tete(pt_objet,__cree_maillon(t,echelle_de_couleur(210)));
    t= definirTriangle3d(tab_points[5],tab_points[3],tab_points[4]);
    __insere_tete(pt_objet,__cree_maillon(t,echelle_de_couleur(210)));
    //face arri�re
    t= definirTriangle3d(tab_points[5],tab_points[6],tab_points[7]);
    __insere_tete(pt_objet,__cree_maillon(t,echelle_de_couleur(520)));
    t= definirTriangle3d(tab_points[5],tab_points[4],tab_points[7]);
    __insere_tete(pt_objet,__cree_maillon(t,echelle_de_couleur(520)));

	return pt_objet;
}

t_objet3d* carre(double l, Uint32 c)
{
    t_objet3d *pt_objet = NULL;
    t_point3d *tab_points[4];
    t_triangle3d *t = NULL;

    pt_objet = objet_vide();

    tab_points[0] = definirPoint3d(0,0,0);
	tab_points[1] = definirPoint3d(l,0,0);
	tab_points[2] = definirPoint3d(l,l,0);
	tab_points[3] = definirPoint3d(0,l,0);

	t = definirTriangle3d(tab_points[0],tab_points[1],tab_points[2]);
    __insere_tete(pt_objet,__cree_maillon(t,c));
    t = definirTriangle3d(tab_points[0],tab_points[2],tab_points[3]);
    __insere_tete(pt_objet,__cree_maillon(t,c));

    return pt_objet;
}

t_objet3d* face(double l, Uint32 c[9])
{
    t_objet3d *pt_objet = objet_vide();
    t_objet3d *otmp = objet_vide();
    int i = 0, lin = 0, col = 0;

    for (i=0;i<9;i++) {
        if (col >= 3) {
            col = 0;
            lin++;
        }
        otmp = carre(l/3,c[i]);
        translationObjet3d(otmp,definirPoint3d(col*l/3,lin*l/3,0));
        composerObjet3d(pt_objet,otmp);
        col++;
    }

    return pt_objet;
}



t_objet3d* rubiks(double l)
{
    t_objet3d *pt_objet = NULL;
    t_objet3d *otmp = NULL;
    t_point3d *origine = definirPoint3d(0,0,0);
    Uint32 c[9];
    int i;
    srand(time(NULL));

    pt_objet = objet_vide();
    otmp = objet_vide();

    //face avant
    c[0] = RC_ROUGE;
    c[1] = RC_ORANGE;
    c[2] = RC_ROUGE;
    c[3] = RC_ORANGE;
    c[4] = RC_ROUGE;
    c[5] = RC_ORANGE;
    c[6] = RC_ROUGE;
    c[7] = RC_ORANGE;
    c[8] = RC_ROUGE;
    otmp = face(l,c);
    composerObjet3d(pt_objet,otmp);

    //faces lat�rales
    c[0] = RC_BLANC;
    c[1] = RC_JAUNE;
    c[2] = RC_BLANC;
    c[3] = RC_JAUNE;
    c[4] = RC_BLANC;
    c[5] = RC_JAUNE;
    c[6] = RC_BLANC;
    c[7] = RC_JAUNE;
    c[8] = RC_BLANC;
    otmp = face(l,c);
    rotationObjet3d(otmp,origine,90,0,0);
    composerObjet3d(pt_objet,otmp);

    c[0] = RC_BLEU;
    c[1] = RC_VERT;
    c[2] = RC_BLEU;
    c[3] = RC_VERT;
    c[4] = RC_BLEU;
    c[5] = RC_VERT;
    c[6] = RC_BLEU;
    c[7] = RC_VERT;
    c[8] = RC_BLEU;
    otmp = face(l,c);
    rotationObjet3d(otmp,origine,0,90,0);
    translationObjet3d(otmp,definirPoint3d(l,0,0));
    composerObjet3d(pt_objet,otmp);

    c[0] = RC_JAUNE;
    c[1] = RC_BLANC;
    c[2] = RC_JAUNE;
    c[3] = RC_BLANC;
    c[4] = RC_JAUNE;
    c[5] = RC_BLANC;
    c[6] = RC_JAUNE;
    c[7] = RC_BLANC;
    c[8] = RC_JAUNE;
    otmp = face(l,c);
    rotationObjet3d(otmp,origine,90,0,0);
    translationObjet3d(otmp,definirPoint3d(0,l,0));
    composerObjet3d(pt_objet,otmp);

    c[0] = RC_VERT;
    c[1] = RC_BLEU;
    c[2] = RC_VERT;
    c[3] = RC_BLEU;
    c[4] = RC_VERT;
    c[5] = RC_BLEU;
    c[6] = RC_VERT;
    c[7] = RC_BLEU;
    c[8] = RC_VERT;
    otmp = face(l,c);
    rotationObjet3d(otmp,origine,0,90,0);
    composerObjet3d(pt_objet,otmp);

    //face arri�re
    c[0] = RC_ORANGE;
    c[1] = RC_ROUGE;
    c[2] = RC_ORANGE;
    c[3] = RC_ROUGE;
    c[4] = RC_ORANGE;
    c[5] = RC_ROUGE;
    c[6] = RC_ORANGE;
    c[7] = RC_ROUGE;
    c[8] = RC_ORANGE;
    otmp = face(l,c);
    translationObjet3d(otmp,definirPoint3d(0,0,l));
    composerObjet3d(pt_objet,otmp);

    return pt_objet;
}

t_objet3d* sphere(double r, double nlat, double nlong)
{
	t_objet3d *pt_objet = NULL;

	pt_objet = objet_vide();

	// TODO

	return pt_objet;
}

t_objet3d* sphere_amiga(double r, double nlat, double nlong)
{
	t_objet3d *pt_objet = NULL;

	pt_objet = objet_vide();

	// TODO


	return pt_objet;
}

t_objet3d* arbre(double lx, double ly, double lz)
{
	t_objet3d *pt_objet = NULL;

	pt_objet = objet_vide();
	// TODO

	return pt_objet;

}

t_objet3d* damier(double lx, double lz, double nx, double nz)
{
	t_objet3d *pt_objet = NULL;

	pt_objet = objet_vide();

	// TODO

	return pt_objet;

}

t_objet3d *copierObjet3d(t_objet3d *o) // attention, effectue une copie mirroir
{
	t_objet3d *n = objet_vide();

	// TODO

	return n;
}

void composerObjet3d(t_objet3d* o, t_objet3d* o2) /* o = o+o2, o2 ne signifiera plus rien */
{
    t_maillon *pt_maillon;
    if (o != NULL && o2 != NULL) {
        if (o->tete == NULL) {
            o->est_trie = o2->est_trie;
            o->tete = o2->tete;
        } else if (o2->tete != NULL) {
            pt_maillon = o->tete;
            while (pt_maillon->pt_suiv != NULL) {
                pt_maillon = pt_maillon->pt_suiv;
            }
            o->est_trie = o->est_trie && o2->est_trie && zmoyen(pt_maillon->face)>=zmoyen((o2->tete)->face);
            pt_maillon->pt_suiv = o2->tete;
        }
    }
}

void libererObjet3d(t_objet3d *o)
{
	// TODO

}

t_maillon *__separer(t_maillon *pt_maillon)
{
    t_maillon * pt_maillon_tmp;

    if (pt_maillon == NULL || pt_maillon->pt_suiv == NULL) {
        return NULL;
    } else {
        pt_maillon_tmp = pt_maillon->pt_suiv;
        pt_maillon->pt_suiv = pt_maillon_tmp->pt_suiv;
        pt_maillon_tmp->pt_suiv = __separer(pt_maillon_tmp->pt_suiv);
        return pt_maillon_tmp;
    }
}

t_maillon *__fusion (t_maillon * pt_maillon1, t_maillon * pt_maillon2)
{
     // On fusionne en comparant les zmoyen des t�tes de listes
        if (pt_maillon1 == NULL) {
            return pt_maillon2;
        } else if (pt_maillon2 == NULL) {
            return pt_maillon1;
        } else if (zmoyen(pt_maillon1->face)>=zmoyen(pt_maillon2->face)) {
            pt_maillon1->pt_suiv = __fusion(pt_maillon1->pt_suiv,pt_maillon2);
            return pt_maillon1;
        } else {
            pt_maillon2->pt_suiv = __fusion(pt_maillon1,pt_maillon2->pt_suiv);
            return pt_maillon2;
        }
}

void __trier_maillons (t_maillon ** ppt_maillon)
{
    t_maillon * pt_maillon_tmp;

    if (*ppt_maillon != NULL && (*ppt_maillon)->pt_suiv != NULL) {
        pt_maillon_tmp = __separer(*ppt_maillon);
        __trier_maillons(ppt_maillon);
        __trier_maillons(&pt_maillon_tmp);
        *ppt_maillon = __fusion(*ppt_maillon,pt_maillon_tmp);
    }

}

void __trier_objet(t_objet3d *pt_objet)
{
    if (pt_objet != NULL && pt_objet->est_trie == false) {
        __trier_maillons(&pt_objet->tete);
        pt_objet->est_trie = true;
    }
}

void dessinerObjet3dAux(t_surface *surface,t_maillon *pt_maillon)
{
    if (pt_maillon != NULL) {
        remplirTriangle3d(surface,pt_maillon->face,pt_maillon->couleur);
        dessinerObjet3dAux(surface,pt_maillon->pt_suiv);
    }
}

void dessinerObjet3d(t_surface *surface, t_objet3d* pt_objet)
{
	if (pt_objet != NULL ) {
        if (!pt_objet->est_trie) {
            __trier_objet(pt_objet);
        }
        dessinerObjet3dAux(surface,pt_objet->tete);
    }
}

void translationObjet3d(t_objet3d* pt_objet, t_point3d *vecteur)
{
    double mt[4][4];
    int i;

	if (pt_objet != NULL && vecteur != NULL) {

        // initialisation de la matrice de translation
        __init_m(mt);
        for(i=0;i<4;i++){
            mt[i][3] = vecteur->xyzt[i];
            mt[i][i] = 1;
        }

        transformationObjet3d(pt_objet,mt);
	}
}

void rotationObjet3d(t_objet3d* pt_objet, t_point3d *centre, float degreX, float degreY, float degreZ)
{
    double m[4][4];
    int i,j;
    float theta;
    __init_m(m);
    m[3][3] = 1;

    __inversion_3(centre);
	translationObjet3d(pt_objet,centre);

	// initialisation de la matrice de rotation selon x
	theta = __conversion_deg_rad(degreX);
    m[0][0] = 1;
    m[1][1] = cos(theta);
    m[1][2] = -1*sin(theta);
    m[2][1] = sin(theta);
    m[2][2] = cos(theta);
    transformationObjet3d(pt_objet,m);

    // initialisation de la matrice de rotation selon y
    theta = __conversion_deg_rad(degreY);
    for(i=0;i<3;i++){
        for(j=0;j<3;j++){
            m[i][j] = 0;
        }
    }
    m[0][0] = cos(theta);
    m[0][2] = -1*sin(theta);
    m[1][1] = 1;
    m[2][0] = sin(theta);
    m[2][2] = cos(theta);
    transformationObjet3d(pt_objet,m);

    // initialisation de la matrice de rotation selon z
    theta = __conversion_deg_rad(degreZ);
    for(i=0;i<3;i++){
        for(j=0;j<3;j++){
            m[i][j] = 0;
        }
    }
    m[0][0] = cos(theta);
    m[0][1] = -1*sin(theta);
    m[1][0] = sin(theta);
    m[1][1] = cos(theta);
    m[2][2] = 1;
    transformationObjet3d(pt_objet,m);

    __inversion_3(centre);
	translationObjet3d(pt_objet,centre);

	// Seule la rotation modifie le zmoyen d'un triangle
	pt_objet->est_trie = false;

}

void transformationObjet3d(t_objet3d* pt_objet, double mat[4][4])
{
    t_maillon *pt_maillon;

    pt_maillon = pt_objet->tete;
	while (pt_maillon != NULL) {
            transformationTriangle3d(pt_maillon->face,mat);
            pt_maillon = pt_maillon->pt_suiv;
    }
}