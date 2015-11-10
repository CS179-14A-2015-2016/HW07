using System;
using SFML.Graphics;
using SFML.System;
using SFML.Window;

namespace Worchbound
{
    class Projectile : CircleShape
    {
        float vx, vy;

        public Projectile(float size)
        {
            this.Radius = size;
        }
        
        public float VX
        {
            get { return this.vx; }
            set { this.vx = value; }
        }

        public float VY
        {
            get { return this.vy; }
            set { this.vy = value; }
        }
    }
    class Program
    {
        static int FPS = 30;
        const float gravity = -9.8f;
        static float v0 = 0;
        static bool isPlayer1Turn = true, isBallInFlight = false;
        static Random windRandomizer = new Random();
        static ContextSettings settings;
        static RenderWindow window;
        static Projectile projectile = new Projectile(5f);
        static RectangleShape player1 = new RectangleShape(new Vector2f(50, 15));
        static RectangleShape player2 = new RectangleShape(new Vector2f(50, 15));
        static RectangleShape player1Gun = new RectangleShape(new Vector2f(25, 5));
        static RectangleShape player2Gun = new RectangleShape(new Vector2f(25, 5));

        static Projectile UpdateProjectilePosition(Projectile projectile, float timeStep)
        {
            Projectile result = new Projectile(5f);

            result.Position = new Vector2f(projectile.Position.X + projectile.VX * timeStep, projectile.Position.Y + projectile.VY * timeStep);
            result.VX = projectile.VX + windRandomizer.Next() * timeStep;
            result.VY = projectile.VY - gravity * timeStep;

            return result;

        }

        static void Main(string[] args)
        {
            v0 = 50;
            Random locationInitializer = new Random();
            settings.AntialiasingLevel = 10;
            window = new RenderWindow(new VideoMode(800, 500), "Worchbound", Styles.Default, settings);
            projectile.Position = new Vector2f(-10f, -10f);
            player1.Position = new Vector2f(locationInitializer.Next(50, 350), locationInitializer.Next(100, 300));
            player2.Position = new Vector2f(locationInitializer.Next(450, 750), locationInitializer.Next(100, 300));
            player1Gun.Position = new Vector2f(player1.Position.X + 25, player1.Position.Y - 5);
            player2Gun.Position = new Vector2f(player2.Position.X + 25, player2.Position.Y);
            player2Gun.Rotation = 180;
            Text text = new Text();
            window.KeyPressed += Window_KeyPressed;
            
            while (window.IsOpen)
            {
                window.DispatchEvents();
                window.Clear();
                if (isBallInFlight)
                {
                    projectile = UpdateProjectilePosition(projectile, 0.016f);
                }
                else if (projectile.Position.Y > 500)
                {
                    isBallInFlight = false;
                    if (isPlayer1Turn)
                    {
                        isPlayer1Turn = false;
                    }
                    else if (!isPlayer1Turn)
                    {
                        isPlayer1Turn = true;
                    }
                }
                else if (isPlayer1Turn)
                {
                    if ((projectile.Position.Y + projectile.Radius >= player1.Position.Y) && (projectile.Position.X >= player1.Position.X) && (projectile.Position.X <= player1.Position.X + player1.Size.X))
                    {
                        window.Close();
                    }
                    if ((projectile.Position.X + projectile.Radius >= player1.Position.X) && (projectile.Position.Y >= player1.Position.Y) && (projectile.Position.Y >= player1.Position.Y + player1.Size.Y))
                    {
                        window.Close();
                    }
                    if ((projectile.Position.X + projectile.Radius >= player2.Position.X) && (projectile.Position.Y <= player2.Position.Y) && (projectile.Position.Y >= player2.Position.Y + player2.Size.Y))
                    {
                        window.Close();
                    }
                }
                else if (!isPlayer1Turn)
                {
                    if ((projectile.Position.Y + projectile.Radius >= player2.Position.Y) && (projectile.Position.X <= player2.Position.X) && (projectile.Position.X >= player2.Position.X + player2.Size.X))
                    {
                        window.Close();
                    }
                    if ((projectile.Position.X + projectile.Radius >= player2.Position.X) && (projectile.Position.Y <= player2.Position.Y) && (projectile.Position.Y >= player2.Position.Y + player2.Size.Y))
                    {
                        window.Close();
                    }
                    if (new Vector2f(-projectile.Position.X - projectile.Position.Y, -projectile.Position.X - projectile.Position.Y) == player2.Position)
                    {
                        window.Close();
                    }
                }
                window.SetTitle(v0.ToString());
                window.Draw(text);
                window.Draw(projectile);
                window.Draw(player1);
                window.Draw(player2);
                window.Draw(player1Gun);
                window.Draw(player2Gun);
                window.Display();
            }
        }

        public static void Window_KeyPressed(object sender, KeyEventArgs e)
        {
            if (e.Code == Keyboard.Key.Space)
            {
                isBallInFlight = true;
                if (isPlayer1Turn)
                {
                    projectile.Position = new Vector2f(player1Gun.Position.X, player1Gun.Position.Y);
                    projectile.VX = v0 * Math.Abs((float)Math.Cos(Math.Abs(player1Gun.Rotation) * Math.PI / 180));
                    projectile.VY = v0 * Math.Abs((float)Math.Sin(Math.Abs(player1Gun.Rotation) * Math.PI / 180));
                }
                else if (!isPlayer1Turn)
                {
                    projectile.Position = new Vector2f(player2Gun.Position.X, player2Gun.Position.Y);
                    projectile.VX = -(v0 * Math.Abs((float)Math.Cos(Math.Abs(player1Gun.Rotation - 180))));
                    projectile.VY = v0 * Math.Abs((float)Math.Sin(Math.Abs(player1Gun.Rotation - 180)));
                }
            } 
            if (e.Code == Keyboard.Key.Left)
            {
                if (isPlayer1Turn)
                {
                    if (!(player1Gun.Rotation <= -90))
                    {
                        player1Gun.Rotation -= 1f;
                    }
                }
                else if (!isPlayer1Turn)
                {
                    if (!(player2Gun.Rotation <= 180))
                    {
                        player2Gun.Rotation -= 1f;
                    }
                }
            }
            if (e.Code == Keyboard.Key.Right)
            {
                if (isPlayer1Turn)
                {
                    if (!(player1Gun.Rotation >= 0))
                    {
                        player1Gun.Rotation += 1f;
                    }
                }
                else if (!isPlayer1Turn)
                {
                    if (!(player2Gun.Rotation >= 270))
                    {
                        player2Gun.Rotation += 1f;
                    }
                }
            }
            if (e.Code == Keyboard.Key.Up)
            {
                v0 += 1;
            }
            if (e.Code == Keyboard.Key.Down)
            {
                v0 -= 1;
            }
        }
    }
}
