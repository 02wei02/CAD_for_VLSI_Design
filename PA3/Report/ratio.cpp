double Constraint::AspectRatio(double w, double h) // aspect ratio
{
    double r1 = w / h;
    double r2 = h / w;
    double ans;
    if (r1 > r2)
        ans = r1;
    else
        ans = r2;
    return ans;
}